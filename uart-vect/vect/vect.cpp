#include "vect.h"

ThrusterMixer::ThrusterMixer() {
    motorPWMs.fill(PWM_NEUTRAL);
}

void ThrusterMixer::calculate(int16_t surge, int16_t sway, int16_t heave, int16_t yaw) {
    int32_t mix[6];

    // --- KARIŞTIRMA MATRİSİ (MIXING MATRIX) ---

    // Motor 1 (Ön Sağ)  : İleri(+), Sağa Kay(-), Sağa Dön(-)
    mix[0] = surge - sway - yaw;
    
    // Motor 2 (Ön Sol)  : İleri(+), Sola Kay(+), Sağa Dön(+)
    mix[1] = surge + sway + yaw;
    
    // Motor 3 (Arka Sağ): İleri(+), Sağa Kay(+), Sağa Dön(-) 
    mix[2] = surge + sway - yaw;
    
    // Motor 4 (Arka Sol): İleri(+), Sola Kay(-), Sağa Dön(+)
    mix[3] = surge - sway + yaw;

    // Motor 5 & 6 (Dikeyler)
    mix[4] = heave;
    mix[5] = heave;

    // --- PWM DÖNÜŞÜMÜ VE SINIRLAMA ---
    for (int i = 0; i < 6; i++) {
        // 1. Yön düzeltmesi
        mix[i] *= directions[i];

        // 2. Merkeze ekle (1500 + etki)
        int32_t pwmVal = PWM_NEUTRAL + mix[i];

        // 3. Sınırla 
        if (pwmVal > PWM_MAX) pwmVal = PWM_MAX;
        if (pwmVal < PWM_MIN) pwmVal = PWM_MIN;

        motorPWMs[i] = static_cast<uint16_t>(pwmVal);
    }
}

const std::array<uint16_t, 6>& ThrusterMixer::getPWMs() const {
    return motorPWMs;
}
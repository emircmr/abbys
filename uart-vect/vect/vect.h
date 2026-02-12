#ifndef VECT_H
#define VECT_H

#include <cstdint>
#include <array>
#include <algorithm>

class ThrusterMixer {
public:
    // PWM limitleri (ESC'ye göre ayarlanacak ornek degerler)
    static constexpr uint16_t PWM_MIN = 1100;
    static constexpr uint16_t PWM_MAX = 1900;
    static constexpr uint16_t PWM_NEUTRAL = 1500;

    ThrusterMixer();


    void calculate(int16_t surge, int16_t sway, int16_t heave, int16_t yaw);


    const std::array<uint16_t, 6>& getPWMs() const;

private:
    std::array<uint16_t, 6> motorPWMs;
    
    // Motor yönleri (1: Normal, -1: Ters Pervane) -test edilecek
    // Bunu test ederek ayarlaman gerekecek!
    const int8_t directions[6] = {1, 1, 1, 1, 1, 1}; 
};

#endif
#include "main.h"
#include "com.h"
#include "vect.h"

// Global Nesneler
SerialParser rovComm;
ThrusterMixer rovMixer;

uint8_t rx_byte;
extern UART_HandleTypeDef huart2; 
extern TIM_HandleTypeDef htim1;   


extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        // 1. Gelen byte'ı parse et
        bool packetReady = rovComm.parseByte(rx_byte);
        
        // 2. Bir sonraki byte için kesmeyi tekrar kur
        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
    }
}


void Apply_PWMs(const std::array<uint16_t, 6>& pwms) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwms[0]); // M1
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwms[1]); // M2
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwms[2]); // M3
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pwms[3]); // M4
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_TIM1_Init();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

    HAL_UART_Receive_IT(&huart2, &rx_byte, 1);

    while (1) {
        const ControlPacket& data = rovComm.getPacket();

        rovMixer.calculate(data.surge, data.sway, data.heave, data.yaw);

        Apply_PWMs(rovMixer.getPWMs());

        HAL_Delay(10);
    }
}
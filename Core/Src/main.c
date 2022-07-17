/*

To generate the interrupt, the interrupt line should be configured and enabled.

This is done by programming the two trigger registers with the desired edge detection and by enabling
the interrupt request by writing a ‘1’ to the corresponding bit in the interrupt mask register.

When the selected edge occurs on the external interrupt line, an interrupt request is generated.

The pending bit corresponding to the interrupt line is also set. This request is
reset by writing a ‘1’ in the pending register.

                 Hardware interrupt selection

To configure the 20 lines as interrupt sources, use the following procedure:
  *Configure the mask bits of the 20 Interrupt lines (EXTI_IMR)

  *Configure the Trigger Selection bits of the Interrupt lines (EXTI_RTSR and EXTI_FTSR)

  *Configure the enable and mask bits that control the NVIC IRQ channel mapped to the
External Interrupt Controller (EXTI) so that an interrupt coming from one of the 20 lines
can be correctly acknowledged.

*/

#include "main.h"

void GPIO_config(void);
void interrupt_config(void);
void EXTI15_10_IRQHandler(void);

int main(void){

	GPIO_config();
	interrupt_config();

  while (1)
  {

  }
}

void GPIO_config(void){

	 RCC->APB2ENR |= ((1<<2) | (1<<4));  // set clock bus PORTA and PORTC
	 GPIOA->CRL = 0X200000; // set PA5 output
	 GPIOC->CRH = 0X800000; // set PC13 input
}

void interrupt_config(void){

	/****      EXTI SETTINGS          ******/

		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // set enable clock bus interrupt
		AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC; // set match PC13 and EXTI Line13
		EXTI->IMR |= EXTI_IMR_MR13; // EXTI LINE13 interrupt mask off
		EXTI->FTSR |= EXTI_FTSR_TR13; // select falling edge trigger

	/****       NVIC SETTINGS     *******/

		NVIC_SetPriority(EXTI15_10_IRQn,1);
		NVIC_EnableIRQ(EXTI15_10_IRQn);

}

void EXTI15_10_IRQHandler(void){

	if (EXTI->PR & 0x2000) {
		EXTI->PR |= EXTI_PR_PR13; // interrupt flag is cleared
		GPIOA->ODR ^= GPIO_ODR_ODR5;
	}


}



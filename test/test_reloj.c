#include "unity.h"
#include "clock.h"

#define TICKS_PER_SECOND 5



/*3) Configurar la biblioteca, ajustar la hora (con valores incorrectos) tiene devolver un error y al consultar la hora tiene que ser invalida.




8) Configurar la hora de la alarma (con valores incorrectos) y revisar si la rechaza.


11) Si la alarma esta desactivada y la activo, queda activa, pero no cambia la hora.

14) Si la se activo, y las pospongo n minutos, vuelve a sonar n minutos
15) Si la se activo, y la cancelo, al día siguiente, vuelve a sonar
16) Si alarma se activa y pasan 23:59:59 no debe volver a activarse, pero si pasa un segundo mas entonces si se debe volver a activar
*/

clock_t reloj;
bool alarm_state;

void simulateSeconds(int seconds){
	for(int index=0; index < seconds * TICKS_PER_SECOND; index++) {
		clockNewTick(reloj);
	}	
}

void alarmEventHandler(clock_t clock, bool state) {
	alarm_state = state;
}

// Creo el reloj y lo pongo en hora
void setUp(void){
	static const uint8_t INICIAL[] = {1, 2, 3, 4};
	reloj =  clockCreate(TICKS_PER_SECOND, alarmEventHandler);
	clockSetUpTime(reloj, INICIAL, sizeof(INICIAL));
	alarm_state = false;
}

//1) Configurar la biblioteca, consultar la hora y tiene que ser invalida.
void test_hora_inicial_invalida (void) {
	static const uint8_t ESPERADO[] = {0, 0, 0, 0, 0, 0};
	uint8_t hora[6];
	uint8_t alarma[4];
	reloj =  clockCreate(TICKS_PER_SECOND, alarmEventHandler);
	
	TEST_ASSERT_FALSE(clockGetTime(reloj, hora, sizeof(hora)));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
	TEST_ASSERT_FALSE(clockGetAlarm(reloj, alarma, sizeof(alarma)));
}

//2) Configurar la biblioteca, ajustar la hora (con valores correctos), consultar la hora y tiene que ser valida.	
void test_set_up_current_time(void) {
	static const uint8_t INICIAL[] = {1, 2, 3, 4};
	static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 0};
	uint8_t hora[6];
	//clock_t reloj = ClockCreate(TICKS_PER_SECOND);
	//ClockSetupTime(reloj, INICIAL, sizeof(INICIAL));
		
	TEST_ASSERT_TRUE(clockGetTime(reloj, hora, sizeof(hora)));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}		

//4) Simular el paso de n ciclos de reloj, consultar la hora y verificar que avanzo un segundo
void test_one_second_elapsed(void){
	static const uint8_t ESPERADO[] = {1, 2, 3, 4, 0, 1};
	uint8_t hora[6];
	
	simulateSeconds(1);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

//5) Simular el paso de 10*n ciclos de reloj, consultar la hora y verificar que avanzo diez segundos
void test_ten_second_elapsed(void){
	static const uint8_t ESPERADO[] = {1, 2, 3, 4, 1, 0};
	uint8_t hora[6];
	
	simulateSeconds(10);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

//6) Simular el paso de un minuto, diez minutos, una hora, diez horas y un día completo.
void test_one_minute_elapsed(void){
	static const uint8_t ESPERADO[] = {1, 2, 3, 5, 0, 0};
	uint8_t hora[6];
	
	simulateSeconds(60);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_ten_minute_elapsed(void){
	static const uint8_t ESPERADO[] = {1, 2, 4, 4, 0, 0};
	uint8_t hora[6];
	
	simulateSeconds(10 * 60);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

void test_one_hour_elapsed(void){
	static const uint8_t ESPERADO[] = {1, 3, 3, 4, 0, 0};
	uint8_t hora[6];
	
	simulateSeconds(3600);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}

/*void test_ten_hour_elapsed(void){
	static const uint8_t ESPERADO[] = {2, 2, 3, 4, 0, 0};
	uint8_t hora[6];
	
	simulateSeconds(36000);
	clockGetTime(reloj, hora, sizeof(hora));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ESPERADO, hora, sizeof(ESPERADO));
}*/

//7) Configurar la hora de la alarma (con valores correctos) y revisar si la guarda.
//9) Configurar la hora de la alarma (con valores correctos) y revisar si la queda activada.
void test_setup_and_get_alarm(void) {
	static const uint8_t ALARMA[] = {1, 2, 3, 5};
	uint8_t hora[4];
	
	clockSetUpAlarm(reloj, ALARMA, sizeof(ALARMA));
	TEST_ASSERT_TRUE(clockGetAlarm(reloj, hora, sizeof(hora)));
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

//10) Si la alarma esta activa y la desactivo, queda desactivada, pero no cambia la hora.
void test_setup_and_disable_alarm(void) {
	static const uint8_t ALARMA[] = {1, 2, 3, 5};
	uint8_t hora[4];
	
	clockSetUpAlarm(reloj, ALARMA, sizeof(ALARMA));
	TEST_ASSERT_FALSE(clockToggleAlarm(reloj));
	
	TEST_ASSERT_FALSE(clockGetAlarm(reloj, hora, sizeof(hora)));// en FALSO queda desactivada
	TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, hora, sizeof(ALARMA));
}

//12) Si la alarma esta activa, y la hora del reloj coincide con la hora de la alarma, entonces suena.

void test_setup_and_fire_alarm(void) {
	static const uint8_t ALARMA[] = {1, 2, 3, 5};
		
	clockSetUpAlarm(reloj, ALARMA, sizeof(ALARMA));
	simulateSeconds(60);
	TEST_ASSERT_TRUE(alarm_state);
	/*alarm_state = false;
	simulateSeconds(10);
	TEST_ASSERT_FALSE(alarm_state);*/
}

//17) Si la alarma esta inactiva no debe sonar cuando la hora del reloj coincida con la hora de la alarma
void test_setup_and_not_fire_alarm(void) {
	static const uint8_t ALARMA[] = {1, 2, 3, 5};
		
	clockSetUpAlarm(reloj, ALARMA, sizeof(ALARMA));
	clockToggleAlarm(reloj);
	
	simulateSeconds(60);
	TEST_ASSERT_FALSE(alarm_state);
}
//Si la activo, y las pospongo n munitos, vuelve a sonar n minutos
//Si la activo, y la cancelo, al dia siguiente vuelve a sonar

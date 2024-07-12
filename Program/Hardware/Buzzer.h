#ifndef _BUZZER_H_
#define _BUZZER_H_

void Buzzer_Init(void);
void Buzzer_Reminder(uint8_t time);
void Buzzer_Warning(uint8_t time);
void Buzzer_Severe(uint8_t time);

#endif

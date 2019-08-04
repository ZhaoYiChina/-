#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f10x.h"
#include <stdio.h>

typedef struct
{
		float X_AcceValue;	//X����ļ��ٶ�
		float Y_AcceValue;	//Y����ļ��ٶ�
		float Z_AcceValue;	//Z����ļ��ٶ�
		
}Acceleration;

typedef struct
{
		float X_AngSpeed;	//X������ٶ�
		float Y_AngSpeed;	//Y������ٶ�
		float Z_AngSpeed;	//Z������ٶ�
}AngularSpeed;

typedef struct
{
		float X_Angle;	//X�Ƕ�
		float Y_Angle;	//Y�Ƕ�
		float Z_Angle;	//Z�Ƕ�
}Angle;

void MPU6050_Init(void);
void MPU6050_RetToZero(void);
#endif

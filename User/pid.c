#include "pid.h"
#include "mpu6050.h"
extern Acceleration  AccelerationMsg;
extern AngularSpeed  AngularSpeedMsg;
extern Angle AngleMsg;

int PIDCalculate(PID_Val *para,PID *p,int now_Val)
{
		int dError;//΢�����
		int Error;//���
		float speedItem;
		int posKpItem,posKiItem,posKdItem;
		/********************�ٶȻ�************************/
		speedItem = para->kpB*AngularSpeedMsg.Z_AngSpeed;
		/*********************λ�û�*****************************/
		Error = p->int_mubiao-now_Val;
		p->int_lastError = p->int_nowError;
		p->int_nowError = Error;
		p->int_sumError += Error;
		dError = p->int_nowError-p->int_lastError;
		posKpItem = para->kpA*Error;
		posKiItem = para->kiA*p->int_sumError;
		posKdItem = para->kdA*dError;
		/****************�������õĵ���*******************/
		return (posKpItem+posKiItem+posKdItem-speedItem);	
}

void PID_Init(PID *pid)
{
		pid->int_lastError = 0;
		pid->int_nowError = 0;
		pid->int_mubiao = 2080;
		pid->int_sumError = 0;
}


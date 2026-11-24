// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include<stdio.h>

#include<hpSimHandlers.h>
#include<carwash.h>

char logHpSim[512] = "";

bool initializationFinished = false;
int  selectedWash = 0;
int  manualCommand = 0;


LOG_HANDLER getLogString(void)
{
	return logHpSim;
}

void sprintfLog(const char* message) {
	strcpy_s(logHpSim, sizeof(logHpSim), message);
}


TRANSITION_HANDLER washIsInitialized() {
	return initializationFinished;
}


TRANSITION_HANDLER arrivedBelow() {

	return isBrushDown();
}

TRANSITION_HANDLER arrivedAbove() {
	return isBrushUp();
}


TRANSITION_HANDLER arrivedAtLeft() {

	return isBrushLeft();
}


TRANSITION_HANDLER arrivedAtRight() {

	return istBrushRight();
}


TRANSITION_HANDLER  contourDetected() {
	return isLeftInfraRedOn() || isRightInfraRedOn();
}

TRANSITION_HANDLER  contourNotDetected() {
	return !contourDetected() && !isBrushDown();
}

//extern "C" __declspec(dllexport) void __cdecl
PLACE_HANDLER  initializingKit(long tokens)
{

	initializationFinished = false;
	initializeCarWash();
	initializationFinished = true;
	// if returning false, 
	// visits HANDLER at every step 

	return true;
}




//PLACE_HANDLER moveRight(long tokens)
PLACE_HANDLER movingRight(long tokens) {
	sprintf_s(logHpSim, sizeof(logHpSim), "doing movingRight:  %ld", tokens);

	stopVerticalMovement();
	moveBrushRight();
	return true;
}


PLACE_HANDLER movingLeft(long tokens)
{
	stopVerticalMovement();
	moveBrushLeft();
	return true;
}


PLACE_HANDLER movingUp(long tokens)
{
	stopHorizMovement();
	moveBrushUp();

	return true;

}

PLACE_HANDLER movingDown(long tokens)
{
	// ensures moveDown has got the right signaure
	CHECK_SIGNATURE_PLACE_HANDLER(movingDown);

	stopHorizMovement();
	moveBrushDown();

	return true;
}


PLACE_HANDLER upUp(long tokens)
{
	moveBrushUp();
	return true;
}

PLACE_HANDLER downDown(long tokens)
{
	moveBrushDown();
	return true;
}


LOG_ALERT_HANDLER alertMissingTransitions(void)
{
	//returning false prevents the annoying messages
	return false;
}

LOG_ALERT_HANDLER alertMissingPlaces(void)
{
	//returning false prevents the annoying messages
	return false;
}

TOKEN_CHANGED_HANDLER onTokenEnterPlace_moveDown(long tokensBefore, long tokensNow)
{
	if (tokensBefore == 0 && tokensNow > 0) {
		//do Something here		
	}
}

TOKEN_CHANGED_HANDLER onTokenLeavePlace_moveDown(long tokensBefore, long tokensNow)
{
	if (tokensBefore >= 0 && tokensNow == 0) {
		//do Something here		
	}

}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


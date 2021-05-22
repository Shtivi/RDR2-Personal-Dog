#pragma once

class PaymentExecutor
{
private:
	ModState* progress;
	MapAreasFactory* mapAreas;
	Prompt* receivePaymentPrompt;
	bool isInPostOffice;

public:
	PaymentExecutor(ModState* progress, MapAreasFactory* mapAreas);
	void update();
	void showPaymentReminder();

private:
	Vector3 findClosestPostOffice();
	Ped getInteractedClerk();
	int collectPayment();
	void onPostOfficeEnterace();
	const char* pendingPaymentDisplay();
};
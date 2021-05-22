#include "Main.h"

PaymentExecutor::PaymentExecutor(ModState* progress, MapAreasFactory* mapAreas)
{
	this->progress = progress;
	this->mapAreas = mapAreas;
	this->isInPostOffice = false;
	this->receivePaymentPrompt = new Prompt("Receive Contracts Payment", GAMEPLAY::GET_HASH_KEY("INPUT_NEXT_CAMERA"), PromptMode::SemiHold);
	receivePaymentPrompt->setTargetEntity(NULL);
	receivePaymentPrompt->hide();
}

void PaymentExecutor::update()
{
	if (progress->getPendingReward() == 0)
	{
		receivePaymentPrompt->setTargetEntity(NULL);
		receivePaymentPrompt->hide();
		return;
	}

	Vector3 closestPostOfficeLoc = findClosestPostOffice();
	if (!closestPostOfficeLoc)
	{
		return;
	}

	if (distance(closestPostOfficeLoc, playerPos()) > 30)
	{
		isInPostOffice = false;
		return;
	}

	if (!isInPostOffice)
	{
		isInPostOffice = true;
		onPostOfficeEnterace();
	}

	Ped clerk = getInteractedClerk();
	if (!clerk)
	{
		receivePaymentPrompt->setTargetEntity(NULL);
		receivePaymentPrompt->hide();
		return;
	}

	receivePaymentPrompt->setTargetEntity(clerk);
	receivePaymentPrompt->show();

	if (receivePaymentPrompt->isActivatedByPlayer())
	{
		int collectedPayment = collectPayment();
		stringstream logMsg;
		logMsg << "Payment Collected: " << collectedPayment;
		log(logMsg.str().c_str());
	}
}

void PaymentExecutor::showPaymentReminder()
{
	stringstream tooltip;
	tooltip << "Contracts Reminder: you have a\npayment of $" << progress->getPendingReward() << " waiting for you at\nthe post office.";
	showHelpText(tooltip.str().c_str());
}

Vector3 PaymentExecutor::findClosestPostOffice()
{
	MapArea* playerMapArea = mapAreas->getMapArea(getPlayerMapArea());
	if (!playerMapArea)
	{
		return toVector3(0, 0, 0);
	}

	return playerMapArea->getClosestPostOfficeTo(playerPos());
}

Ped PaymentExecutor::getInteractedClerk()
{
	Entity clerk;
	if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &clerk))
	{
		int model = ENTITY::GET_ENTITY_MODEL(clerk);
		if (model == 3915992895 || model == 1057932105 || model == -378974401 || model == -972643259 || model == 292812873)
		{
			return (Ped)clerk;
		}
	}

	return NULL;
}

int PaymentExecutor::collectPayment()
{
	int payment = progress->getPendingReward();
	CASH::PLAYER_ADD_CASH(payment * 100, 752097756);
	progress->clearReward();
	stringstream money;
	money << "Contracts payment collected: $" << payment;
	showSubtitle(money.str().c_str());
	return payment;
}

void PaymentExecutor::onPostOfficeEnterace()
{ 
	showPaymentReminder();
}

const char* PaymentExecutor::pendingPaymentDisplay()
{
	stringstream money;
	money << "$" << progress->getPendingReward();
	return money.str().c_str();
}
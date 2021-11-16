#include "pch.h"
#include "Script.hpp"

Vehicle currentVehicle = 0;
bool flyingActive;

void ShowHelpTextThisFrame(char* text)
{
	char textEntry[] = "STRING";
	UI::_SET_TEXT_COMPONENT_FORMAT(textEntry);
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DISPLAY_HELP_TEXT_FROM_STRING_LABEL(0, 0, false, -1);
}

void main()
{
	while (true)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			Vehicle handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			Hash model = ENTITY::GET_ENTITY_MODEL(handle);

			if (VEHICLE::IS_THIS_MODEL_A_CAR(model))
			{
				currentVehicle = handle;
			}
			else
			{
				if (flyingActive || currentVehicle != 0)
				{
					flyingActive = false;
					currentVehicle = 0;
				}
			}
		}
		else
		{
			if (flyingActive || currentVehicle != 0)
			{
				flyingActive = false;
				currentVehicle = 0;
			}
		}

		if (currentVehicle != 0)
		{
			if (!flyingActive)
			{
				ShowHelpTextThisFrame((char*)"Press ~INPUT_VEH_DUCK~ to enable flying mode");

				if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, 73))
				{
					flyingActive = true;
				}
			}
			else
			{
				ShowHelpTextThisFrame((char*)"Press ~INPUT_VEH_GRAPPLING_HOOK~ to go up\nPress ~INPUT_VEH_DUCK~ to disable flying mode");
			
				if (CONTROLS::IS_CONTROL_PRESSED(0, 103))
				{
					Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(currentVehicle);
					ENTITY::SET_ENTITY_VELOCITY(currentVehicle, currentVelocity.x, currentVelocity.y, 45);
				}

				if (ENTITY::IS_ENTITY_IN_AIR(currentVehicle) && CONTROLS::IS_CONTROL_PRESSED(0, 71))
				{
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(currentVehicle, 45);
				}

				if (CONTROLS::IS_CONTROL_JUST_RELEASED(0, 73))
				{
					flyingActive = false;
				}
			}
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

void OnAbort()
{
	
}

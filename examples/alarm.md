# Alarms

You can send alarms via the thingboard platform to your web interface or even to other devices.

## Task

1. Connect your arduino to the thingsboard server
2. Send the RSSI value every second to the server
3. Make the arduinos builtin led blink at 0.7 Hz
4. Also send a temperature measurement as telemetry to the server (0.6 Hz) (this is a random value between 70 and 86 in this example)
5. Create a new alarm rule
   1. if the temperature is above 80, send a critical alarm
   2. make sure the alarm is propagated to the tenant and customer
   3. once the temperature is below 75, clear the alarm
6. Create a second alarm rule
   1. Once the WiFi RSSI is below -55, send a warning
   2. make sure the alarm is propagated to the tenant and customer
   3. once the WiFi RSSI is above -50, clear the alarm

## Setup IoT Server

1. Try opening the servers Webpage and login with your account
2. On the left side click "Alarms" and then "Alarm rules"
3. Add a new alarm rule
   1. Set a unique alarm type
   2. Set your device properly
   3. Select the corresponding telemetry data as "Arguments"
   4. Set the trigger condition
   5. Set the clear condition
   6. Under "advanced settings" make sure to propagate the alarm
4. Configure the other alarm

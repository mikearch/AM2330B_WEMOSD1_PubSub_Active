void pubTemp() {

// Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

    // Read temperature as Celsius (the default)

   float t = (th.cTemp);


    // Check if any reads failed and exit early (to try again).
    if (isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
      static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);   // changed hic variable in this function to t



    // Publishes Temperature and Humidity values
    // esp1 = living room
    // esp2 = Outside
    // esp3 = Bar

    client.publish("home/office/esp2/temperature", temperatureTemp);
    Serial.print ("Published Temp = ");
    Serial.println (temperatureTemp);
    // client.publish("home/office/esp1/humidity", humidityTemp); //comented out humidity stuff

    return;
}

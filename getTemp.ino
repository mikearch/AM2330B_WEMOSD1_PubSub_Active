 //Read data and store it to variables hum and temp
    
    void getTemp(){
   
  Serial.println("Chip = AM2320");
  switch(th.Read()) {
    case 2:
      Serial.println("  CRC failed");
      break;
    case 1:
      Serial.println("  Sensor offline");
      break;
    case 0:
      Serial.print("  Humidity = ");
      Serial.print(th.Humidity);
      hum = (th.Humidity);
      Serial.println("%");
      Serial.print("  Temperature = ");
      Serial.print(th.cTemp);
      temp = (th.cTemp);
      Serial.println("*C");
      Serial.println();
      break;
  }
  //delay(2000);
  return;
}

#define LED 2

String receivedMessage = "";
int cont = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available()) {
    char incomingChar = Serial.read();  // Read each character from the buffer
    
    if (incomingChar == '\n') {
      // Print the message
      digitalWrite(LED, HIGH);
      Serial.print("You sent: ");
      Serial.println(receivedMessage);
      delay(1000);
      digitalWrite(LED, LOW);
      
      // Clear the message buffer for the next input
      receivedMessage = "";
      cont = 0;
    } else {
      // Append the character to the message string
      digitalWrite(LED, HIGH);
      receivedMessage += incomingChar;
      delay(200);
      digitalWrite(LED, LOW);
    }
  }
}


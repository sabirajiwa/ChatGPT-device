#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Replace with your network credentials
const char* ssid = "CampusNet";            // Enter your Wi-Fi SSID
const char* password = "";     // Enter your Wi-Fi Password

// Replace with your OpenAI API key
const char* apiKey = "Bearer Ardiuno D1 Mini Chat GPT";       // Enter your OpenAI API Key
const char* apiURL = "https://api.openai.com/v1/chat/completions"; // ChatGPT API URL
void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected.");
}

void loop() {
  // Check if there is incoming data
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n'); // Read user input
    Serial.print("User Input: ");
    Serial.println(userInput); // Display the input

    // Send the user input to ChatGPT
    String response = sendChatMessage(userInput);
    Serial.println("ChatGPT Response: ");
    Serial.println(response); // Display the response
  }
  delay(100); // Short delay to avoid flooding the Serial Monitor
}

// Function to send a message to ChatGPT
String sendChatMessage(String message) {
  String responseContent = ""; // To store the response

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client; // Create a WiFiClient object

    // Print the URL for debugging
    Serial.println("Connecting to: " + String(apiURL));

    // Initialize the HTTP connection
    if (!http.begin(client, apiURL)) {
      Serial.println("HTTP begin failed");
      return "HTTP begin failed"; // Return an error message
    }

    // Set the content type and authorization headers
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", apiKey); // Ensure "Bearer " prefix is included

    // Prepare the JSON payload
    String payload = String("{\"model\":\"gpt-3.5-turbo\",\"messages\":[{\"role\":\"user\",\"content\":\"") + message + "\"}]}";

    // Send POST request
    int httpResponseCode = http.POST(payload);

    // Check the response code
    if (httpResponseCode > 0) {
      responseContent = http.getString(); // Get the response
      Serial.println("Response code: " + String(httpResponseCode));
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
      responseContent = "Error sending request"; // Return an error message
    }

    // Close the connection
    http.end();
  } else {
    Serial.println("Wi-Fi not connected");
    responseContent = "Wi-Fi not connected"; // Return an error message
  }

  return responseContent; // Return the response
}

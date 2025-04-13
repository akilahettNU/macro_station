// html_content.h
#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html { 
      font-family: Helvetica; 
      display: inline-block; 
      margin: 0px auto; 
      text-align: center;
    }
    .button { 
      background-color: #4CAF50; 
      border: none; 
      color: white; 
      padding: 16px 40px;
      text-decoration: none; 
      font-size: 30px; 
      margin: 2px; 
      cursor: pointer;
    }
    select {
      font-size: 20px;
      padding: 10px;
      margin: 10px;
      width: 80%;
      max-width: 300px;
    }
    .container {
      border: 1px solid #ccc;
      border-radius: 10px;
      padding: 20px;
      margin: 20px auto;
      max-width: 500px;
      background-color: #f8f8f8;
    }
    h2 {
      color: #333;
    }
  </style>
</head>
<body>
  <h1>ESP32 HID Controller</h1>
  
  <div class="container">
    <h2>Button Configuration</h2>
    <p>Select Win + Key combination for the action button:</p>
    <form action="/set-button" method="get">
      <select name="keycombo" id="keycombo">
        <option value="d" %SELECTED_D%>Win + D (Show Desktop)</option>
        <option value="e" %SELECTED_E%>Win + E (File Explorer)</option>
        <option value="p" %SELECTED_P%>Win + P (Project/Display)</option>
        <option value="r" %SELECTED_R%>Win + R (Run)</option>
        <option value="l" %SELECTED_L%>Win + L (Lock)</option>
      </select>
      <p><button type="submit" class="button">Save</button></p>
    </form>
    <p>Current action button function: Win + %CURRENT_KEY%</p>
  </div>
  
  <div class="container">
    <h2>Device Info</h2>
    <p>Press the WiFi button (GPIO 7) during boot to configure WiFi settings</p>
    <p>Press the action button (GPIO 5) to trigger Win + %CURRENT_KEY%</p>
  </div>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H
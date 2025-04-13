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
    .button2 {
      background-color: #555555;
    }
  </style>
</head>
<body>
  <h1>ESP32 Web Server</h1>
  
  <p>GPIO 12 - State %OUTPUT12_STATE%</p>
  <p><a href="%BUTTON12_ACTION%"><button class="%BUTTON12_CLASS%">%BUTTON12_TEXT%</button></a></p>
  
  <p>GPIO 14 - State %OUTPUT14_STATE%</p>
  <p><a href="%BUTTON14_ACTION%"><button class="%BUTTON14_CLASS%">%BUTTON14_TEXT%</button></a></p>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H
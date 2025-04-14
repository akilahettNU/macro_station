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
      padding: 12px 30px;
      text-decoration: none;
      font-size: 18px;
      margin: 2px;
      cursor: pointer;
      border-radius: 4px;
    }
    select {
      font-size: 16px;
      padding: 8px;
      margin: 5px;
      width: 80%;
      max-width: 250px;
    }
    .container {
      border: 1px solid #ccc;
      border-radius: 10px;
      padding: 15px;
      margin: 15px auto;
      max-width: 600px;
      background-color: #f8f8f8;
    }
    .button-config {
      border: 1px solid #ddd;
      border-radius: 8px;
      padding: 10px;
      margin: 10px auto;
      background-color: #fff;
      max-width: 500px;
    }
    h2 {
      color: #333;
      margin-top: 10px;
    }
    h3 {
      color: #555;
      margin: 5px 0;
    }
    .matrix-layout {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 10px;
      margin: 10px auto;
      max-width: 500px;
    }
  </style>
</head>
<body>
  <h1>ESP32 HID Controller</h1>
  
  <div class="container">
    <h2>Button Matrix Configuration</h2>
    <p>Select Win + Key combination for each button:</p>
    
    <form action="/set-button" method="get">
      <div class="matrix-layout">
        <div class="button-config">
          <h3>Button 1 (Top Left)</h3>
          <select name="button1" id="button1">
            <option value="d" %SELECTED_D_1%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_1%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_1%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_1%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_1%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_1%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_1%</p>
        </div>
        
        <div class="button-config">
          <h3>Button 2 (Top Right)</h3>
          <select name="button2" id="button2">
            <option value="d" %SELECTED_D_2%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_2%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_2%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_2%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_2%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_2%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_2%</p>
        </div>
        
        <div class="button-config">
          <h3>Button 3 (Middle Left)</h3>
          <select name="button3" id="button3">
            <option value="d" %SELECTED_D_3%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_3%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_3%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_3%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_3%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_3%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_3%</p>
        </div>
        
        <div class="button-config">
          <h3>Button 4 (Middle Right)</h3>
          <select name="button4" id="button4">
            <option value="d" %SELECTED_D_4%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_4%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_4%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_4%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_4%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_4%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_4%</p>
        </div>
        
        <div class="button-config">
          <h3>Button 5 (Bottom Left)</h3>
          <select name="button5" id="button5">
            <option value="d" %SELECTED_D_5%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_5%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_5%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_5%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_5%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_5%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_5%</p>
        </div>
        
        <div class="button-config">
          <h3>Button 6 (Bottom Right)</h3>
          <select name="button6" id="button6">
            <option value="d" %SELECTED_D_6%>Win + D (Show Desktop)</option>
            <option value="e" %SELECTED_E_6%>Win + E (File Explorer)</option>
            <option value="p" %SELECTED_P_6%>Win + P (Project/Display)</option>
            <option value="r" %SELECTED_R_6%>Win + R (Run)</option>
            <option value="l" %SELECTED_L_6%>Win + L (Lock)</option>
            <option value="m" %SELECTED_M_6%>Win + M (Minimize All)</option>
          </select>
          <p>Current: Win + %CURRENT_KEY_6%</p>
        </div>
      </div>
      
      <p><button type="submit" class="button">Save All Button Settings</button></p>
    </form>
  </div>
  
  <div class="container">
    <h2>Device Info</h2>
    <p>Press the WiFi button (GPIO 7) during boot to configure WiFi settings</p>
    <p>Buttons are arranged in a 3×2 matrix (3 rows × 2 columns)</p>
    <p>Rows: GPIO 9, 10, 11 | Columns: GPIO 4, 5</p>
  </div>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H
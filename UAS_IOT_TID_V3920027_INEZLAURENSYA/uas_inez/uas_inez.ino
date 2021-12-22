// Present a "Will be back soon web page", as stand-in webserver.
// 2011-01-30 <jc@wippler.nl>
//
// License: GPLv2

#include <EtherCard.h>

#define STATIC 1  // set ke 1 untuk menonaktifkan DHCP (sesuaikan nilai myip/gwip di bawah)

#if STATIC
// alamat ip antarmuka ethernet
static byte myip[] = { 192,168,1,200 };
// alamat ip gateway
static byte gwip[] = { 192,168,1,1 };
#endif

// alamat mac ethernet - harus unik 
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip mengirim dan menerima buffer
// teks yang akan ditampilkan kedalam website
const char page[] PROGMEM =
"HTTP/1.0 503 My Service \r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "UAS IOT"
  "</title></head>"
  "<body>"
    "<h3>PRAKTIK INTERNET OF THINGS</h3>"
    "<p><em>"
      "INEZ LAURENSYA<br />"
      "V3920027 / TID<br />"
    "</em></p>"
  "</body>"
"</html>"
;

void setup(){
  Serial.begin(9600);
  Serial.println("\n[backSoon]");

  // mengubah 'SS' ke pin Slave Select , jika  tidak menggunakan pin default
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0)
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
}

void loop(){
  // tunggu paket TCP masuk
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}

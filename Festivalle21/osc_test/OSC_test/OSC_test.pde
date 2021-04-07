import oscP5.*;
import netP5.*;


OscP5 osc;
NetAddress remoteLocation;

void setup() {
  size(200,200);
  osc = new OscP5(this, 9001);
  }

void draw() {
background(0);
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.arguments()[0]);
}

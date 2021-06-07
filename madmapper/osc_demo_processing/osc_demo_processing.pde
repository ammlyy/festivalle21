import oscP5.*;
import netP5.*;


OscP5 osc;
NetAddress remoteLocation;
float R, G, B = 0.0;
void setup() {
  size(200,200);
  osc = new OscP5(this, 5005);
  }

void draw() {
  background(R, G, B);
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage msg) {
  /* print the address pattern and the typetag of the received OscMessage */
  if(msg.checkAddrPattern("/juce/RGB")==true) {
      R = msg.get(0).floatValue();  
      G = msg.get(1).floatValue();  
      B = msg.get(2).floatValue();  

      print("### received an osc message /test with typetag ifs.");
      println(" values: "+R+" "+G+" "+B+" ");
      return;
  }

}

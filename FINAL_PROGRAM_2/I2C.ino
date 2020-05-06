void sendAction(int mode) {
	//Wire.beginTransmission(MAPLE_ADDR);
	//Wire.write(mode);
	//Wire.endTransmission();
}

bool checkLightGates() {
	if(analogRead(A10) < FRONT_GATE_THRESH) {
		//sendAction(ON_FRONT_DRIB);
		return true;
	} else {
		return false;
	}
	/*if(analogRead(A11) < BACK_GATE_THRESH) {
		sendAction(ON_BACK_DRIB);
		angular_drive(0,0,0);
	}*/
}

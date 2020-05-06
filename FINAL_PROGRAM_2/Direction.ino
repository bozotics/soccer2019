void calcBallAngle() {
	ballDist = sqrt(pow(ball[1]-CAMERA_CENTER_Y,2)+pow(ball[2]-CAMERA_CENTER_X,2));		//temp raw ball distance, unscaled
	ballAng = acos((ball[1]-CAMERA_CENTER_Y)/ballDist)/pi*180;
	//ballAng = mod((180-ballAng),360);
	if(ball[2] >= CAMERA_CENTER_X) ballAng = 360-ballAng;

	//ballAng=mod(ballAng-90,360);

	ballDist = 0.00005*pow(ballDist,3) - 0.0111*pow(ballDist,2) + 1.4712*ballDist - 29.753;
}

void calcRobotAngle() {
	float ballFactor = 1-ballDist/BALL_DIST_MAX;	//convert ball distance to a percentage of closeness
	ballMult = fmin(0.002*pow(e,7.0*ballFactor),1);
	/*
	if(ballAng>90 && ballAng<270) {	//back
			ballOffset = ballAng-180;
		// Serial.print("back ");
		// if(ballAng<180) {
		// 	robotAng = mod((mod(ballAng-180,360) + ballOffset*ballMult),360);
		// 	desiredAngle = 0;	
		// }
		// else {
		// 	robotAng = mod((mod(ballAng-180,360) + ballOffset*ballMult),360);
		// 	desiredAngle = 0;
		// }
			robotAng = ballAng + ballOffset*ballMult;
		Serial.print("back ");

	} else {
		if(ballAng<180) ballOffset = fmin(ballAng,90);
		else ballOffset = fmax(ballAng-360,-90);

		robotAng = ballAng + ballOffset*ballMult;
		Serial.print("front ");
	}*/
	if(ballAng<180) ballOffset = fmin(ballAng,90);
	else ballOffset = fmax(ballAng-360,-90);

	robotAng = ballAng + ballOffset*ballMult;
}

void calcGoalAngle() {
	if(ygoal[0] == 0) {	//no yellow goal
		yGoalAng = -1;
		yGoalDist = -1;
	} else {
		float yGoalCtrX = (ygoal[2]+ygoal[4])/2.0;
		float yGoalCtrY = (ygoal[1]+ygoal[2])/2.0;
		yGoalDist = sqrt(pow(yGoalCtrY-CAMERA_CENTER_Y,2)+pow(yGoalCtrX-CAMERA_CENTER_X,2));
		yGoalAng = acos((yGoalCtrY-CAMERA_CENTER_Y)/yGoalDist)/pi*180;
		yGoalAng=360-mod(ballAng+180,360);

		if(yGoalCtrX <= CAMERA_CENTER_X) yGoalAng = 360-yGoalAng;

		//Serial.printf("%d %d %d %d %f %f %f %f ", ygoal[1], ygoal[2], ygoal[3], ygoal[4],yGoalCtrX, yGoalCtrY, yGoalAng, ballAng);
	}

	if(bgoal[0] == 0) {	//no blue goal

	} else {
		float bGoalCtrX = (bgoal[2]+bgoal[4])/2.0;
		float bGoalCtrY = (bgoal[1]+bgoal[3])/2.0;
    bGoalDist = sqrt(pow(bGoalCtrY-CAMERA_CENTER_Y,2)+pow(bGoalCtrX-CAMERA_CENTER_X,2));    //temp raw ball distance, unscaled
    bGoalAng = acos((bGoalCtrY-CAMERA_CENTER_Y)/bGoalDist)/pi*180;
	
		if(bGoalCtrX >= CAMERA_CENTER_X) bGoalAng = 360-bGoalAng;
   //Serial.printf("%f %f %f\t",bGoalCtrX,bGoalCtrY,bGoalAng);
	}
}

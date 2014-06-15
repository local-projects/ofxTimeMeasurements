/*
 *  ofxTimeMeasurements.h
 *  emptyExample
 *
 *  Created by Oriol Ferrer Mesià on 28/01/11.
 *  Copyright 2011 uri.cat. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include <map>

#define TIME_MEASUREMENTS_LINE_HEIGHT		14
#define TIME_MEASUREMENTS_EDGE_GAP_H		10
#define TIME_MEASUREMENTS_EDGE_GAP_V		5
#define TIME_MEASUREMENTS_LINE_H_MULT		0.25

#define TIME_MEASUREMENTS_UPDATE_KEY		"update()"
#define TIME_MEASUREMENTS_DRAW_KEY			"draw()"

#define TIME_SAMPLE_SET_FRAMERATE(x)	(ofxTimeMeasurements::instance()->setDesiredFrameRate(x))
#define TIME_SAMPLE_START(x)			(ofxTimeMeasurements::instance()->startMeasuring(x))
#define TIME_SAMPLE_STOP(x)				(ofxTimeMeasurements::instance()->stopMeasuring(x))
#define TIME_SAMPLE_SET_DRAW_LOCATION(x,...)(ofxTimeMeasurements::instance()->setDrawLocation(x,##__VA_ARGS__))
#define TIME_SAMPLE_GET_ENABLED()		(ofxTimeMeasurements::instance()->getEnabled())
#define TIME_SAMPLE_SET_ENABLED(e)		(ofxTimeMeasurements::instance()->setEnabled(e))
#define TIME_SAMPLE_ENABLE()			(ofxTimeMeasurements::instance()->setEnabled(true))
#define TIME_SAMPLE_DISABLE()			(ofxTimeMeasurements::instance()->setEnabled(false))
#define TIME_SAMPLE_SET_AVERAGE_RATE(x)	(ofxTimeMeasurements::instance()->setTimeAveragePercent(x)) /* 1.0 means no averaging, 0.01 means each new sample only effects 1% on previous sample */
#define TIME_SAMPLE_DISABLE_AVERAGE()	(ofxTimeMeasurements::instance()->setTimeAveragePercent(1))
#define TIME_SAMPLE_SET_PRECISION(x)	(ofxTimeMeasurements::instance()->setMsPrecision(x)) /* how many precion digits to show on time measurements */

enum ofxTMDrawLocation{	TIME_MEASUREMENTS_TOP_LEFT,
	TIME_MEASUREMENTS_TOP_RIGHT,
	TIME_MEASUREMENTS_BOTTOM_LEFT,
	TIME_MEASUREMENTS_BOTTOM_RIGHT,
	TIME_MEASUREMENTS_CUSTOM_LOCATION
};

class ofxTimeMeasurements: public ofBaseDraws {

	public :

		static ofxTimeMeasurements* instance();
	
		void setDesiredFrameRate(float fr);	//forced to do this as I can't access desiredFrameRate once set with ofSetFrameRate
											//affects the % busy indicator
		void startMeasuring(string ID);
		void stopMeasuring(string ID);
		void setEnabled( bool enable );
		bool getEnabled();
		void setDrawLocation(ofxTMDrawLocation loc, ofVec2f p = ofVec2f()); //p only relevant if using TIME_MEASUREMENTS_CUSTOM_LOCATION
		void setMsPrecision(int digits);		//how many decimals for the ms units
		void setTimeAveragePercent(float p);	//[0..1] >> if set to 1.0, 100% of every new sample contributes to the average.
												//if set to 0.1, a new sample contributes 10% to the average
		unsigned long durationForID( string ID);
		void setBgColor(ofColor c){bgColor = c;}
		void setHighlightColor(ofColor c){hiColor = c;}
		void setTextColor(ofColor c){textColor = c;}
	
		virtual float getWidth(){ return maxW * 8; }
		virtual float getHeight(){ return ( TIME_MEASUREMENTS_LINE_H_MULT * 5 + times.size() + 1 ) * TIME_MEASUREMENTS_LINE_HEIGHT; };

	private:

		ofxTimeMeasurements(); // use ofxTimeMeasurements::instance() instead!

		struct TimeMeasurement{
			unsigned long microsecondsStart;
			unsigned long microsecondsStop;
			unsigned long duration;
			float avgDuration;
			bool measuring;
			bool error;
			bool updatedLastFrame;
			int level; //for nested measurements
			TimeMeasurement(){
				level = 0;
			}
		};

		void _beforeUpdate(ofEventArgs &d){startMeasuring(TIME_MEASUREMENTS_UPDATE_KEY);};
		void _afterUpdate(ofEventArgs &d){stopMeasuring(TIME_MEASUREMENTS_UPDATE_KEY);};
		void _beforeDraw(ofEventArgs &d){startMeasuring(TIME_MEASUREMENTS_DRAW_KEY);};
		void _afterDraw(ofEventArgs &d){stopMeasuring(TIME_MEASUREMENTS_DRAW_KEY); autoDraw(); };

		void draw(float x, float y);
		void draw(float x, float y, float w , float h){ cout << "ofxTimeMeasurements: ignoring draw() call" << endl; } //w and h ignored! just here to comply with ofBaseDraws

		void autoDraw();
		void updateSeparator();

		static ofxTimeMeasurements*		singleton;
		float							desiredFrameRate;
		bool							enabled;

		map<string, TimeMeasurement>	times;
		map<int, string>				keyOrder;

		int								stackLevel; //for Nested measurements

		float							timeAveragePercent;
		int								msPrecision;
		string							TIME_SAMPLE_SEPARATOR;

		ofxTMDrawLocation				drawLocation;
		ofVec2f							loc;
		int								maxW; //for a text line
		int								longestLabel; //

		ofColor							bgColor;
		ofColor							hiColor;
		ofColor							textColor;

		string							lastKey;
};


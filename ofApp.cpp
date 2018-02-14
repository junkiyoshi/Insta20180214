#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	ofSetFrameRate(30);
	ofBackground(255);
	ofSetWindowTitle("Insta");

	this->size = 5;

	this->colors.push_back(ofColor(0, 117, 201));
	this->colors.push_back(ofColor(255, 164, 0));
	this->colors.push_back(ofColor(38, 39, 42));
	this->colors.push_back(ofColor(0, 149, 58));
	this->colors.push_back(ofColor(236, 0, 68));

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	this->values.clear();

	float x_count = ofGetWidth() / this->size;
	float y_count = ofGetHeight() / this->size;

	for (int y = 0; y < y_count; y++) {

		vector<float> tmp_values;

		for (int x = 0; x < x_count; x++) {

			if (y == 0) {

				float value = ofMap(ofNoise(x * 0.05, ofGetFrameNum() * 0.05), 0, 1, this->size, 0);
				tmp_values.push_back(value);
			}
			else {

				float value = 0;
				int count = 0;

				for (int tmp_x = x - 1; tmp_x <= x + 1; tmp_x++) {

					if (tmp_x >= 0 && tmp_x < x_count) {

						value += this->values[y - 1][tmp_x];
						count++;
					}
				}

				if (y >= 2) {

					value += this->values[y - 2][x];
					count++;
				}

				float average = value / count + ofMap(ofNoise(x * 0.03, y * 0.1 - ofGetFrameNum() * 0.5), 0, 1, -3, 3);
				tmp_values.push_back(average);
			}
		}

		this->values.push_back(tmp_values);
	}

	this->fbo.begin();
	ofClear(0);

	int color_index = 0;
	int span = ofGetWidth() / this->colors.size();
	for (int x = 0; x < ofGetWidth(); x += span) {

		ofSetColor(this->colors[color_index]);
		color_index++;

		ofDrawRectangle(ofVec2f(x, 0), x + span, ofGetHeight());
	}

	ofSetColor(255);
	for (int y = 0; y < this->values.size(); y++) {

		for (int x = 0; x < this->values[y].size(); x++) {

			ofDrawCircle(ofVec2f(x * this->size + this->size / 2, ofGetHeight() - (y * this->size + this->size / 2)), this->size - this->values[y][x]);
		}
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	this->fbo.draw(0, 0);
}

//========================================================================
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
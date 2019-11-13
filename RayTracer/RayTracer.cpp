#include "RayTracer.h"

float d;
float pw;

void main() {
	readScene("SphereFlake1.scn");
}

void readScene(string filename) {
	ifstream infile(filename);
	string line;
	float r, g, b, x, y, z;
	while (getline(infile, line)) {
		auto firstSpace = line.find_first_of(' ');
		if (firstSpace < 0) continue;
		istringstream iss(line.substr(firstSpace + 1));

		if (line[0] == '#') {
			// comment, do nothing
		}
		else if (startsWith(line, "group")) {

		}
		else if (startsWith(line, "groupend")) {

		}
		else if (startsWith(line, "view")) {
			float n, d;
			iss >> n >> d;
			setView(n, d);
		}
		else if (startsWith(line, "sphere")) {
			sphere();
		}
		else if (startsWith(line, "scale")) {
			iss >> x >> y >> z;
			scale(x, y, z);
		}
		else if (startsWith(line, "move")) {
			iss >> x >> y >> z;
			move(x, y, z);
		}
		else if (startsWith(line, "rotate")) {
			float angle;
			iss >> angle >> x >> y >> z;
			rotate(angle, x, y, z);
		}
		else if (startsWith(line, "light")) {
			iss >> r >> g >> b;
			iss >> x >> y >> z;
			light(r, g, b, x, y, z);
		}
		else if (startsWith(line, "background")) {
			iss >> r >> g >> b;
			background(r, g, b);
		}
		else if (startsWith(line, "ambient")) {
			iss >> r >> g >> b;
			ambient(r, g, b);
		}
		else if (startsWith(line, "material")) {
			float dr, dg, db, sr, sg, sb, p;
			iss >> dr >> dg >> db >> sr >> sg >> sb >> p;
			material(dr, dg, db, sr, sg, sb, p);
		}
		else if (startsWith(line, "refraction")) {
			float i;
			iss >> r >> g >> b >> i;
			refraction(r, g, b, i);
		}
	}
}

void setView(float nPixel, float distance) {
	d = distance;
	pw = 2 * distance / nPixel;
	cout << "view d:  " << d << "\tpw: " << pw << endl;
}

void sphere() {
	cout << "sphere\n";
}

void scale(float sx, float sy, float sz) {
	cout << "scale: " << sx << " " << sy << " " << sz << endl;
}

void move(float x, float y, float z) {
	cout << "move: " << x << " " << y << " " << z << endl;
}

void rotate(float angle, float x, float y, float z) {
	cout << "rotate: " << angle << " " << x << " " << y << " " << z << endl;
}

void light(float r, float g, float b, float x, float y, float z) {
	cout << "light: " << r << " " << g << " " << b 
		<< x << " " << y << " " << z << endl;
}

void background(float r, float g, float b) {
	cout << "background: " << r << " " << g << " " << b << endl;
}

void ambient(float r, float g, float b) {
	cout << "ambient: " << r << " " << g << " " << b << endl;
}

void material(float dr, float dg, float db, float sr, float sg, float sb, float p) {
	cout << "material: " << dr << " " << dg << " " << db 
		<< sr << " " << sg << " " << sb << endl;
}

void refraction(float r, float g, float b, float i) {
	cout << "refraction: " << r << " " << g << " " << b << " " << i << endl;
}

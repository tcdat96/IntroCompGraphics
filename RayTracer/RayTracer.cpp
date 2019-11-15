#include "RayTracer.h"

float d;
int n;
float pSize;

vec3 gBackground = vec3(0, 0, 0);
Material gMaterial;

vec3 gCamera(0, 0, 4);

vector<Sphere*> gSpheres;
vec3** gPixels = nullptr;

int main() {
	if (!readScene("SphereFlake.scn")) {
		cout << "There was a problem reading scene file.";
		return 0;
	}

	float halfSize = pSize / 2;
	float iX = -d + halfSize;
	float iY = d - halfSize;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Ray ray(gCamera);
			ray.v = vec3(iX + i * pSize, iY - j * pSize, 0) - gCamera;
			gPixels[i][j] = trace(ray);
		}
	}

	exportPpm(gPixels, n, n);

	for (auto sphere : gSpheres) {
		delete sphere;
	}

	return 1;
}

vec3 trace(Ray& ray) {
	findClosestIntersection(ray);
	return ray.sphere != nullptr ? shade(ray) : gBackground;
}

void findClosestIntersection(Ray& ray) {
	for (auto sphere : gSpheres) {
		float t = sphere->findIntersection(ray);
		if (t < 0) continue;
		if (t < ray.t) {
			ray.t = t;
			ray.sphere = sphere;
		}
	}
}

vec3 shade(const Ray& ray) {
	return ray.sphere->getMaterial().diffuse;
}

bool readScene(string filename) {
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
			int n;
			float d;
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

	return gPixels != nullptr;
}

void exportPpm(vec3** pixels, int xSize, int ySize) {
	FILE* ppm;
	fopen_s(&ppm, "out.ppm", "wb");
	if (ppm == nullptr) {
		cout << "There was a problem opening out.ppm" << endl;
		exit(0);
	}

	fprintf(ppm, "P6\n# %dx%d Raytracer output\n%d %d\n255\n",
		xSize, ySize, xSize, ySize);
	for (int j = ySize - 1; j >= 0; j--) { // Y is flipped!
		for (int i = 0; i < xSize; i++) {
			int r = pixels[i][j][0] * 255;
			int g = pixels[i][j][1] * 255;
			int b = pixels[i][j][2] * 255;
			//cout << r << " " << g << " " << b << endl;
			fprintf(ppm, "%c%c%c", r, g, b);
		}
	}

	fclose(ppm);

#ifdef DEBUG
	cout << "\n\n\n";
	ifstream infile("out.ppm");
	string line;
	while (getline(infile, line)) {
		cout << line << endl;
	}
#endif
}

void setView(int nPixel, float distance) {
	if (nPixel <= 0 || distance <= 0) {
		return;
	}

	d = distance;
	n = nPixel;
	pSize = 2 * distance / nPixel;

	gPixels = new vec3 * [n];
	for (int i = 0; i < n; i++) {
		gPixels[i] = new vec3[n];
	}
}

void sphere() {
	Sphere* sphere = new Sphere(gMaterial);
	gSpheres.push_back(sphere);
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
	gBackground = vec3(r, g, b);
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

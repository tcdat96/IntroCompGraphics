#include "RayTracer.h"

float d;
int n;

vec3 gCamera(0, 0, 1);

dvec3 gBackground = vec3(0, 0, 0);
vector<Light> gLights;
dvec3 gAmbient;

vector<Group> gGroups;
vector<Sphere*> gSpheres;
dvec3** gPixels = nullptr;



int main() {
	if (!readScene("SphereFlake1.scn")) {
		cout << "There was a problem reading scene file.";
		return 0;
	}

	float pSize = 2 * d / n;
	float halfSize = pSize / 2;
	float iX = -d + halfSize;
	float iY = d - halfSize;
	float scale = glm::tan(glm::radians(60.0f));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Ray ray(gCamera);
			float x = iX + j * pSize;
			float y = iY - i * pSize;
			ray.v = vec3(x * scale, y * scale, 0) - gCamera;
			gPixels[i][j] = trace(ray);
		}
	}

	exportPpm(gPixels, n, n);

	for (auto sphere : gSpheres) {
		delete sphere;
	}

	return 1;
}

dvec3 trace(const Ray& ray) {
	Surface* surface = findClosestIntersection(ray);
	if (surface != nullptr) {
		dvec3 color = !gLights.empty() ? shade(ray, surface) : gBackground;
		delete surface;
		return color;
	}
	return gBackground;
}

Surface* findClosestIntersection(const Ray& ray) {
	double tNear = DBL_MAX;
	Sphere* sphereNear = nullptr;
	for (auto sphere : gSpheres) {
		double t = sphere->findIntersection(ray);
		if (t < 0) continue;
		if (t < tNear) {
			tNear = t;
			sphereNear = sphere;
		}
	}

	if (sphereNear != nullptr) {
		dvec3 p = ray.u + ray.v * tNear;
		dvec3 normal = sphereNear->computeNormal(p);
		return new Surface(p, normal, sphereNear);
	}
	return nullptr;
}

dvec3 shade(const Ray& ray, Surface* surface) {
	dvec3 color = gAmbient;

	// workaround for cancer
	auto p = surface->adjustedHitPoint();

	for (Light light : gLights) {
		if (isShadow(p, light)) continue;
		color += PhongIllumination(ray, surface, light);
	}

	// reflection
	if (surface->sphere->isReflected() && ray.depth < MAX_RAY_DEPTH) {
		Ray reflect(p, glm::reflect(ray.v, surface->normal), ray.depth + 1);
		color += 0.2 * trace(reflect);
	}

	return color;
}

bool isShadow(dvec3 hitPoint, const Light& light) {
	Ray shadowRay(hitPoint, light.position - hitPoint);
	for (auto sphere : gSpheres) {
		double t = sphere->findIntersection(shadowRay);
		if (t >= 0) {
			return true;
		}
	}
	return false;
}

dvec3 PhongIllumination(const Ray& ray, Surface* surface, const Light& light) {
	Material material = surface->sphere->getMaterial();
	
	// diffuse
	dvec3 lightDir = glm::normalize(light.position - surface->hitPoint);
	double lambertian = max(glm::dot(surface->normal, lightDir), 0.0);

	// specular
	double specular = 0;
	if (lambertian > 0) {
		dvec3 viewDir = glm::normalize(-ray.v);
		dvec3 reflectDir = glm::reflect(-lightDir, surface->normal);
		specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	dvec3 color = lambertian * material.diffuse + specular * material.specular;
	return color * light.color;
}

// scene
bool readScene(string filename) {
	gGroups.push_back(Group());

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
			gGroups.push_back(Group(gGroups.back()));
		}
		else if (startsWith(line, "groupend")) {
			gGroups.pop_back();
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

void exportPpm(dvec3** pixels, int xSize, int ySize) {
	FILE* ppm;
	fopen_s(&ppm, "out.ppm", "wb");
	if (ppm == nullptr) {
		cout << "There was a problem opening out.ppm" << endl;
		exit(0);
	}

	fprintf(ppm, "P6\n# %dx%d Raytracer output\n%d %d\n255\n",
		xSize, ySize, xSize, ySize);
	//for (int j = ySize - 1; j >= 0; j--) { // Y is flipped!
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int r = int(min(pixels[i][j][0], 1.0) * 255);
			int g = int(min(pixels[i][j][1], 1.0) * 255);
			int b = int(min(pixels[i][j][2], 1.0) * 255);
			//cout << (pixels[i][j][0] == 0.0 ? 1 : 0) << " ";
			fprintf(ppm, "%c%c%c", r, g, b);
		}
		//cout << endl;
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

	gPixels = new dvec3 * [n];
	for (int i = 0; i < n; i++) {
		gPixels[i] = new dvec3[n];
	}
}

void sphere() {
	Group group = gGroups.back();
	Sphere* sphere = new Sphere(group.xfm, group.material);
	gSpheres.push_back(sphere);
}

void addTransformation(mat4 xfm) {
	Group& group = gGroups.back();
	group.xfm = group.xfm * xfm;
}

void scale(float sx, float sy, float sz) {
	mat4 xfm = glm::scale(mat4(1), vec3(sx, sy, sz));
	addTransformation(xfm);
}

void move(float x, float y, float z) {
	mat4 xfm = glm::translate(mat4(1), vec3(x, y, z));
	addTransformation(xfm);
}

void rotate(float angle, float x, float y, float z) {
	//mat4 xfm = glm::rotate(mat4(1), angle, vec3(x, y, z));
	//addTransformation(xfm);
}

void light(float r, float g, float b, float x, float y, float z) {
	Light light(dvec3(x, y, z), dvec3(r, g, b));
	gLights.push_back(light);
}

void background(float r, float g, float b) {
	gBackground = vec3(r, g, b);
}

void ambient(float r, float g, float b) {
	gAmbient = dvec3(r, g, b);
}

void material(float dr, float dg, float db, float sr, float sg, float sb, float p) {
	Material material = Material(dvec3(dr, dg, db), dvec3(sr, sg, sb), p);
	gGroups.back().material = material;
}

void refraction(float r, float g, float b, float i) {
	cout << "refraction: " << r << " " << g << " " << b << " " << i << endl;
}

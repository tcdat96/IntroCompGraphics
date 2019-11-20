#include "RayTracer.h"

float d;
int n;

vec3 gCamera(0, 0, 1);

dvec3 gBackground = vec3(0, 0, 0);
vector<Light> gLights;
dvec3 gAmbient;
vector<Refraction*> gRefracted;

vector<Group> gGroups;
vector<Sphere*> gSpheres;
dvec3** gPixels = nullptr;

AntiAlias gMode = AntiAlias::NONE;
int gSubdivision = DEFAULT_SAMPLING_SUBDIVISION;
int gProgress = 0;

bool gHasMotion = false;

int main() {
	if (!readScene("SphereFlake1.scn")) {
		cout << "There was a problem reading scene file.";
		return 0;
	}

	Refraction* scene = new Refraction(vec3(0), AIR_COEFFICENT);
	gRefracted.push_back(scene);

	if (!gHasMotion) {
		castRays();
	} 
	else {
		for (int i = 0; i < MOTION_BLUR_ITERATION; i++) {
			castRays();
			for (auto sphere : gSpheres) {
				sphere->updateMotion();
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				gPixels[i][j] /= MOTION_BLUR_ITERATION;
			}
		}
	}

	exportPpm(gPixels, n, n);

	cleanUp();
	return 1;
}

void castRays() {
	gProgress = 0;

	auto start = chrono::high_resolution_clock::now();
	// casting primary rays
	switch (gMode)
	{
	case AntiAlias::SUPER_SAMPLING: {
		castRaysSuperSampling();
		break;
	}
	case AntiAlias::ADAPTIVE_SAMPLING:
		castRaysAdaptiveSuperSampling();
		break;
	case AntiAlias::NONE:
	default: {
		castRaysDefault();
		break;
	}
	}
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<std::chrono::seconds>(end - start).count();
	cout << "\rTime spent: " << duration << "s" << endl;
}

void castRaysDefault() {
	float pSize = 2 * d / n;
	float halfSize = pSize / 2;
	float iX = -d + halfSize;
	float iY = d - halfSize;

	Ray ray(gCamera);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float x = iX + j * pSize;
			float y = iY - i * pSize;
			ray.depth = 0;
			ray.v = vec3(x, y, 0) - gCamera;
			gPixels[i][j] += trace(ray);

			printProgress(i, j);
		}
	}
}

void castRaysSuperSampling() {
	float pSize = 2 * d / n;

	Ray ray(gCamera);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float subSize = pSize / gSubdivision;
			float iX = -d + pSize * j + subSize / 2;
			float iY = d - pSize * i - subSize / 2;
			auto color = dvec3(0);
			for (int k = 0; k < gSubdivision; k++) {
				for (int l = 0; l < gSubdivision; l++) {
					float x = iX + l * subSize;
					float y = iY - k * subSize;
					ray.depth = 0;
					ray.v = vec3(x, y, 0) - gCamera;
					color += trace(ray);
				}
			}
			color /= gSubdivision * gSubdivision;
			gPixels[i][j] += color;
			printProgress(i, j);
		}
	}
}

void castRaysAdaptiveSuperSampling() {
	float pSize = 2 * d / n;

	int percentage = 0;
	int pixels = n * n;

	Ray ray(gCamera);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			float x = -d + j * pSize;
			float y = d - i * pSize;
			gPixels[i][j] += castRayAdaptive(x, y, pSize);
			printProgress(i, j);
		}
	}
}

dvec3 castRayAdaptive(float left, float top, float size, int depth) {
	Ray ray(gCamera);
	float bottom = top + size;
	float right = left + size;

	dvec3 tl = traceAdaptive(ray, left, top);
	dvec3 tr = traceAdaptive(ray, right, top);
	dvec3 bl = traceAdaptive(ray, left, bottom);
	dvec3 br = traceAdaptive(ray, right, bottom);

	if (!similar(tl, tr, bl, br) && depth < gSubdivision) {
		float midV = top + size / 2;
		float midH = left + size / 2;
		float halfSize = size / 2;
		tl = castRayAdaptive(left, top, halfSize, depth + 1);
		tr = castRayAdaptive(midH, top, halfSize, depth + 1);
		bl = castRayAdaptive(left, midV, halfSize, depth + 1);
		br = castRayAdaptive(midH, midV, halfSize, depth + 1);
	}

	return (tl + tr + bl + br) / 4.0;
}

dvec3 traceAdaptive(Ray& ray, float x, float y) {
	static std::unordered_map<string, dvec3> colorMap;

	// little hack to obtain the key
	int factor = 10000000;
	auto key = to_string(int(x * factor)) + to_string(int(y * factor));
	auto value = colorMap.find(key);
	if (value != colorMap.end()) {
		return value->second;
	} 

	// limit memory
	if (colorMap.size() > (1 << 12)) {
		colorMap.clear();
	}
	dvec3 color = trace(ray, x, y);
	colorMap.insert(make_pair(key, color));
	return color;
}

dvec3 trace(Ray& ray, float x, float y) {
	ray.depth = 0;
	ray.v = vec3(x, y, 0) - gCamera;
	return trace(ray);
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
	Intersection* nearest = nullptr;
	for (auto sphere : gSpheres) {
		Intersection* curInt = sphere->findIntersection(ray);
		if (curInt != nullptr) {
			if (nearest == nullptr || curInt->t < nearest->t) {
				nearest = curInt;
			}
			else {
				delete curInt;
			}
		}
	}

	if (nearest != nullptr) {
		dvec3 p = ray.u + ray.v * nearest->t;
		auto sphere = nearest->sphere;
		dvec3 normal = sphere->computeNormal(nearest);
		return new Surface(p, normal, sphere);
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
	auto sphere = surface->sphere;
	if (sphere->isReflected() && ray.depth < MAX_RAY_DEPTH) {
		Ray reflect(p, glm::reflect(ray.v, surface->normal), ray.depth + 1);
		color += sphere->getMaterial().kr * trace(reflect);
	}

	// refraction
	//color += calcRefraction(ray, surface, sphere->getRefraction());

	return color;
}

bool isShadow(dvec3 hitPoint, const Light& light) {
	Ray shadowRay(hitPoint, light.position - hitPoint);
	for (auto sphere : gSpheres) {
		auto curInt = sphere->findIntersection(shadowRay);
		if (curInt != nullptr) {
			return true;
		}
	}
	return false;
}

dvec3 PhongIllumination(const Ray& ray, Surface* surface, const Light& light) {
	Material material = surface->sphere->getMaterial();
	
	// diffuse
	dvec3 lightDir = glm::normalize(light.position - surface->hitPoint);
	double lambertian = glm::max(glm::dot(surface->normal, lightDir), 0.0);

	// specular
	double specular = 0;
	if (lambertian > 0) {
		dvec3 viewDir = glm::normalize(-ray.v);
		dvec3 reflectDir = glm::reflect(-lightDir, surface->normal);
		specular = pow(glm::max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}

	dvec3 color = lambertian * material.getDiffuse(surface->hitPoint) + specular * material.specular;
	return color * light.color;
}

dvec3 calcRefraction(const Ray& ray, Surface* surface, Refraction* curRef) {
	dvec3 color = dvec3(0);
	bool existed = find(gRefracted.begin(), gRefracted.end(), curRef) != gRefracted.end();
	if ((existed || !equals(curRef->eta, gRefracted.back()->eta)) && ray.depth < MAX_RAY_DEPTH) {
		double eta = 0;
		if (existed) {
			gRefracted.erase(std::remove(gRefracted.begin(), gRefracted.end(), curRef), gRefracted.end());
			eta = gRefracted.back()->eta / curRef->eta;
		}
		else {
			eta = curRef->eta / gRefracted.back()->eta;
			gRefracted.push_back(curRef);
		}

		dvec3 refractRay = glm::refract(ray.v, surface->normal, eta);
		if (refractRay != dvec3(0)) {
			dvec3 u = surface->adjustedHitPoint(existed);
			Ray refract(u, refractRay, ray.depth + 1);
			color += 0.2 * trace(refract);
		}
	}
	return color;
}

void printProgress(int i, int j) {
	static int nPixels = n * n;
	int progress = (i * n + j) * 100 / nPixels;
	if (progress > gProgress) {
		cout << "\r" << progress << "%";
		gProgress = progress;
	}
}

void cleanUp() {
	delete gRefracted[0];
	for (auto sphere : gSpheres) {
		delete sphere;
	}
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
		else if (startsWith(line, "groupend")) {
			gGroups.pop_back();
		}
		else if (startsWith(line, "group")) {
			gGroups.push_back(Group(gGroups.back()));
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
			float kr = DEFAULT_REFLECTION_COEFFICENT;
			iss >> dr >> dg >> db >> sr >> sg >> sb >> p >> kr;
			material(dr, dg, db, sr, sg, sb, p, kr);
		}
		else if (startsWith(line, "refraction")) {
			float i;
			iss >> r >> g >> b >> i;
			refraction(r, g, b, i);
		}
		else if (startsWith(line, "texture")) {
			int mode;
			iss >> mode;
			texture(mode);
		}
		else if (startsWith(line, "antialias")) {
			int mode, subdivision = DEFAULT_SAMPLING_SUBDIVISION;
			iss >> mode >> subdivision;
			antiAlias(mode, subdivision);
		}
		else if (startsWith(line, "motionBlur")) {
			bool enabled = true;
			iss >> enabled;
			gGroups.back().motionBlur = enabled;
			if (enabled) gHasMotion = true;
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
	for (int i = 0; i < xSize; i++) {
		for (int j = 0; j < ySize; j++) {
			int r = int(std::min(pixels[i][j][0], 1.0) * 255);
			int g = int(std::min(pixels[i][j][1], 1.0) * 255);
			int b = int(std::min(pixels[i][j][2], 1.0) * 255);
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

	gPixels = new dvec3 * [n];
	for (int i = 0; i < n; i++) {
		gPixels[i] = new dvec3[n];
		for (int j = 0; j < n; j++) {
			gPixels[i][j] = dvec3(0);
		}
	}
}

void sphere() {
	Group& group = gGroups.back();
	Sphere* sphere = new Sphere(group.xfm, group.material, group.refraction, group.motionBlur);
	gSpheres.push_back(sphere);
}

void addTransformation(mat4 xfm) {
	Group& group = gGroups.back();
	group.xfm = group.xfm * xfm;
	//group.xfm = xfm * group.xfm;
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
	mat4 xfm = glm::rotate(mat4(1), radians(angle), vec3(x, y, z));
	addTransformation(xfm);
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

void material(float dr, float dg, float db, float sr, float sg, float sb, float p, float kr) {
	Material material(dvec3(dr, dg, db), dvec3(sr, sg, sb), p, kr);
	Group& group = gGroups.back();
	material.procTexture = group.material.procTexture;
	group.material = material;
}

void refraction(float r, float g, float b, float i) {
	Group& group = gGroups.back();
	group.refraction = Refraction(dvec3(r, g, b), i);
}

void texture(int mode) {
	switch (mode) {
	case 0:
		gGroups.back().material.procTexture = false;
		break;
	case 1:
		gGroups.back().material.procTexture = true;
		break;
	}
}

void antiAlias(int mode, int subdivision) {
	gMode = (AntiAlias)mode;
	gSubdivision = std::min(std::max(subdivision, 0), MAX_SAMPLING_SUBDIVISION);
}
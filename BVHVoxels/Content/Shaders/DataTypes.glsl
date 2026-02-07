struct Ray {
	vec4 origin;
	vec4 direction;
	vec3 reciprocalDirection;
};

struct RayHitInfo {
	bool hit;
	vec4 point;
	float distance;
	
	vec3 color;
};


struct BVNode {
	vec3 min;
	uint left;
	vec3 max;
	uint right;
};

struct TransformationData {
	mat4 transformation;
	mat4 inverseTransformation;
};

struct ModelData {
	vec3 offset;
	uint paletteIndex;
	ivec3 size;
	uint dataIndex;
};
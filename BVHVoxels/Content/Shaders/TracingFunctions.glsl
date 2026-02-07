Ray TransformRay(Ray original, mat4 transformation) {
	Ray ray;
	ray.origin = transformation * original.origin;

	ray.direction = transformation * original.direction;

	ray.reciprocalDirection = 1 / ray.direction.xyz;

	return ray;
}

Ray RayFromScreenPosition(vec2 screenPosition) {
	Ray ray;

	ray.origin = vec4(0,0,0,1);

	ray.direction = vec4((screenPosition / resolution - 0.5) * 2, 2, 1);
	ray.direction.x *= -window_aspect_ratio;
	ray.direction = inverseProjectionMatrix * ray.direction;
	ray.direction /= ray.direction.w;
	ray.direction.w = 0;

	ray.direction.xyz = normalize(ray.direction.xyz);

	ray = TransformRay(ray, viewMatrix);

	return ray;
}




RayHitInfo RayAABB(Ray ray, vec3 boxMin, vec3 boxMax) {

	vec3 tbot = ray.reciprocalDirection * (boxMin - ray.origin.xyz);
	vec3 ttop = ray.reciprocalDirection * (boxMax - ray.origin.xyz);

	vec3 tmin = min(ttop, tbot);
	vec3 tmax = max(ttop, tbot);

	vec2 t = max(tmin.xx, tmin.yz);

	float t0 = max(t.x, t.y);
	
	t = min(tmax.xx, tmax.yz);

	float t1 = min(t.x, t.y);

	RayHitInfo rayHitInfo;

	rayHitInfo.hit = t1 > max(t0, 0.0);

	rayHitInfo.point = vec4(ray.origin.xyz + ray.direction.xyz * t0, 1.0);

	rayHitInfo.distance = t0;
	rayHitInfo.color = vec3(1.0 / log(t1+1), 0.0, 0.0);

	return rayHitInfo;
}

RayHitInfo RayModel(Ray ray, uint modelIndex) {
	ModelData modelData = modelDatas[modelIndex];

	RayHitInfo rayHitInfo = RayAABB(ray, modelData.offset, modelData.size + modelData.offset);

	return rayHitInfo;
}

RayHitInfo RayObject(Ray ray, uint objectIndex) {
	TransformationData transformationData = transformationDatas[objectIndex];

	Ray transformedRay = TransformRay(ray, transformationData.inverseTransformation);
	
	RayHitInfo rayHitInfo = RayModel(transformedRay, objectModels[objectIndex]);

	rayHitInfo.point = transformationData.transformation * rayHitInfo.point;

	rayHitInfo.distance = length(rayHitInfo.point - ray.origin);
	rayHitInfo.color = vec3(0.0, 0.0, 1.0 / log(rayHitInfo.distance+1));

	return rayHitInfo;
}

RayHitInfo RayWorld(Ray ray) {

	RayHitInfo rayHitInfo;
	rayHitInfo.hit = false;
	rayHitInfo.point = vec4(0.0, 0.0, 0.0, 1.0);
	rayHitInfo.distance = 99999;
	rayHitInfo.color = vec3(0.0, 0.0, 0.0);

	uint bvhStack[32];

	bvhStack[0] = bvRootNodeIndex;

	uint bvhStackIndex = 0;

	float depth = 0;

	while(bvhStackIndex != -1) {

		if(bvhStackIndex >= 30){
			return rayHitInfo;
		}

		BVNode bvNode = bvNodes[bvhStack[bvhStackIndex]];

		bvhStackIndex--;

		RayHitInfo bvNodeHitInfo = RayAABB(ray, bvNode.min, bvNode.max);
		
		if(bvNodeHitInfo.hit) {
			if(bvNodeHitInfo.distance < rayHitInfo.distance) {

				depth++;

				if(bvNode.right == -1) {

					RayHitInfo objectHitInfo = RayObject(ray, bvNode.left);

					if(objectHitInfo.hit) {
						if(objectHitInfo.distance < rayHitInfo.distance) {
							rayHitInfo = objectHitInfo;
						}
					}
				}
				else {
					vec3 leftOffset = (bvNodes[bvNode.left].min+bvNodes[bvNode.left].max)/2.0-ray.origin.xyz;
					vec3 rightOffset = (bvNodes[bvNode.right].min+bvNodes[bvNode.right].max)/2.0-ray.origin.xyz;
					
					if(dot(leftOffset,leftOffset) < dot(rightOffset,rightOffset)){
						bvhStackIndex++;
						bvhStack[bvhStackIndex] = bvNode.right;
						bvhStackIndex++;
						bvhStack[bvhStackIndex] = bvNode.left;
					}else{
						bvhStackIndex++;
						bvhStack[bvhStackIndex] = bvNode.left;
						bvhStackIndex++;
						bvhStack[bvhStackIndex] = bvNode.right;
					}
				}
			}
		}
	}

	if(!rayHitInfo.hit) {
		rayHitInfo.color = vec3(depth/128.0, 0.0, 0.0);
	}

	return rayHitInfo;
}
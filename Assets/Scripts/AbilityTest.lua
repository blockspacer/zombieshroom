ACTION_CREATE = 0;
ACTION_COLLIDE = 1;
ACTION_DESTROY = 2;

AbilityTest = {};

function AbilityTest.Process (action, ...)

	local args = table.pack(...);

	if action == ACTION_CREATE then
		AbilityTest.OnCreate();
	elseif action == ACTION_COLLIDE then
		AbilityTest.OnCollide(args);
	elseif action == ACTION_DESTROY then
		AbilityTest.OnDestroy(args);
	end
end

function AbilityTest.OnCreate ()
	local playerEnt = Entity.GetEntityByTag("Player");
	local posVec = Entity.GetEntityByTag("Player"):GetTransformation():GetPos();
	local frontVec = Entity.GetEntityByTag("AimingDevice"):GetTransformation():GetOrient():GetFront();

	local entity0 = Entity.New();

	local renderComp0 = Renderable.New(entity0);
	local collisionComp0 = Collision.New(entity0);
	local colRespComp0 = CollisionResponder.New(entity0);
	local physicsComp0 = Physics.New(entity0);
	collisionComp0:CreateHandle(entity0:GetId(), 1, false);
	local transformComp0 = Transformation.New(entity0);
	local scriptComp0 = Script.New(entity0, "AbilityTest");
	local entity1 = Entity.New();

	local renderComp1 = Renderable.New(entity1);
	local collisionComp1 = Collision.New(entity1);
	local colRespComp1 = CollisionResponder.New(entity1);
	local physicsComp1 = Physics.New(entity1);
	collisionComp1:CreateHandle(entity1:GetId(), 1, false);
	local transformComp1 = Transformation.New(entity1);
	local scriptComp1 = Script.New(entity1, "AbilityTest");
	transformComp0:SetPos(posVec);
	renderComp0:SetModel("Primitives/sphereTangents");
	renderComp0:SetMaterial("Fireball");
	renderComp0:SetMaterialDiffuse("fireballDiffuse");
	renderComp0:SetMaterialSpecular("fireballSpecular");
	renderComp0:SetMaterialNormal("fireballNormal");
	renderComp0:SetMaterialEffect("Mesh_NormalMap");
	physicsComp0:BindShape(collisionComp0, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 5, 1, true);
	physicsComp0:SetVelocity(collisionComp0, Vec3.New(frontVec.x * 20, frontVec.y * 20, frontVec.z * 20));
	colRespComp0:SetContainer(collisionComp0);
	transformComp1:SetPos(posVec);
	renderComp1:SetModel("Primitives/sphereTangents");
	renderComp1:SetMaterial("Fireball");
	renderComp1:SetMaterialDiffuse("fireballDiffuse");
	renderComp1:SetMaterialSpecular("fireballSpecular");
	renderComp1:SetMaterialNormal("fireballNormal");
	renderComp1:SetMaterialEffect("Mesh_NormalMap");
	physicsComp1:BindShape(collisionComp1, Vec3.New((posVec.x + frontVec.x * 3), (4 + posVec.y + frontVec.y * 3), (posVec.z + frontVec.z * 3)), Quat.New(0,0,0,1), 1, 1, true);
	physicsComp1:SetVelocity(collisionComp1, Vec3.New(frontVec.x * 10, frontVec.y * 10, frontVec.z * 10));
	colRespComp1:SetContainer(collisionComp1);
end

function AbilityTest.OnCollide (args)
end

function AbilityTest.OnDestroy (args)
end


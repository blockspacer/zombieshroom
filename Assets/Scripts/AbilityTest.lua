AbilityTest = {};
AbilityTest.damage = 100;
AbilityTest.knockback = 5;
AbilityTest.cooldown = 1;
AbilityTest.charges = 5;
AbilityTest.chargeTime = 0;
AbilityTest.channelingTime = 0;
AbilityTest.duration = 50;

function AbilityTest.ChargeDone (time, userId, actionId)
	AbilityTest.OnCreate(userId, actionId);
end

function AbilityTest.ChannelingDone (time, userId, actionId)
end

function AbilityTest.OnCreate (userId, actionId)
	--Entities
	local self = Entity.New();
	local rayEnt = Entity.New();
	local casterEnt = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 0);
	local networkEnt = Network.New(self, userId, actionId);
	--Components
	local transformComp = Transformation.New(self);
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local scriptComp = Script.New(self, "AbilityTest");
	local timerComp = Timer.New(self, AbilityTest.duration);
	--local rayTimerComp = Timer.New(rayEnt, 2);
	local homingComp = Homing.New(self, 0.1, 50);
	--Setting stuff
	collisionComp:CreateHandle(self, 1, false);
	colRespComp:SetContainer(collisionComp);
	local dirVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetUp();
	local tempPos = casterEnt:GetTransformation():GetPos() + Vec3.New(0,1,0);
	local startPos = Vec3.New((tempPos.x + dirVec.x * 3), (2 + tempPos.y + dirVec.y * 3), (tempPos.z + dirVec.z * 3));
	local dirVecForward = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local rayStartPos = Vec3.New((tempPos.x + dirVecForward.x * 3), (2 + tempPos.y + dirVecForward.y * 3), (tempPos.z + dirVecForward.z * 3));
	-- Get the position which is aimed at
	local rayComp = Ray.New(rayEnt, collisionComp:GetHandle(), rayStartPos, dirVecForward, 10000, false, false);
	rayComp = Ray.New(rayEnt, collisionComp:GetHandle(), tempPos, rayComp:GetHitPos() - tempPos, 10000, true, false);
	local entityAtAim = rayComp:GetHitEntity();
	if entityAtAim:DoesExist() then
		local type = entityAtAim:GetPhysics():GetType(entityAtAim:GetCollision());
		if type == PhysicsType.TYPE_PLAYER then
			local abilityOwnerNetwork = self:GetNetwork();
			local abilityOwnerId = abilityOwnerNetwork:GetUserId();
			local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
			local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
			if abilityOwnerPlayerComponent:GetTeamId() ~= entityAtAim:GetPlayerComponent():GetTeamId() then
				homingComp:SetTargetEntity(entityAtAim);
			end
		end
	else
		--local pos = rayComp:GetHitPos();
		homingComp:SetTargetPosition(rayComp:GetHitPos());
	end
	physicsComp:BindSphereShape(collisionComp, startPos, Quat.New(0,0,0,1), 1, 1, true, true);
	physicsComp:SetVelocity(collisionComp, Vec3.New(dirVec.x * 50, dirVec.y * 50, dirVec.z * 50));
	physicsComp:SetGravity(collisionComp, Vec3.New(0, -9.82, 0));
	transformComp:SetPos(startPos);

	if Global.IsClient then
		local renderComp = Renderable.New(self);
		renderComp:SetModel("Primitives/sphereTangents");
		renderComp:SetMaterial("Fireball");
		renderComp:SetMaterialDiffuse("fireballDiffuse");
		renderComp:SetMaterialSpecular("fireballSpecular");
		renderComp:SetMaterialNormal("fireballNormal");
		renderComp:SetMaterialEffect("Mesh_NormalMap");
		local particleComp = ParticleEmitter.New(self, "fireball");
	end
end

function AbilityTest.OnCollide (self, entity)
	local hitCol = entity:GetCollision();
	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
	if type == PhysicsType.TYPE_PLAYER then
		local targetPlayerComponent = entity:GetPlayerComponent();
		local abilityOwnerNetwork = self:GetNetwork();
		local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			local health = entity:GetHealth();
			if not health:IsDead() then
				local network = entity:GetNetwork();
				local receiverId = network:GetUserId();
				health:Damage(abilityOwnerId, AbilityBall.damage, receiverId);
			end
			AbilityTest.OnDestroy(self);
		end
		if abilityOwnerPlayerComponent:GetTeamId() == targetPlayerComponent:GetTeamId() then
			local hitPos = entity:GetTransformation():GetPos();
			local selfPos = self:GetTransformation():GetPos();
			hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), AbilityTest.knockback);
		end
	end
end
--Logging.Log(LogLevel.DEBUG_PRINT, "OnCollide");
function AbilityTest.OnDestroy (self)
	Entity.Remove(self);
end
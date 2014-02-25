Explosion = {};
Explosion.damage = 60;
Explosion.pushback = 20;
Explosion.cooldown = 0;
Explosion.chargeTime = 0.0;
Explosion.channelingTime = 0.0;
Explosion.duration = 1;

function Explosion.OnCreate (userId, actionId)
	--Logging.Log(LogLevel.DEBUG_PRINT, "Creating Explosion");
	local self = Entity.New();
	local fatherEntity = Entity.GetEntityByNetworkID(userId, actionId, 0);
	local prevHandle = fatherEntity:GetCollision():GetHandle();
	--Logging.Log(LogLevel.DEBUG_PRINT, "Handle in Expl: "..prevHandle);
	local posVec = fatherEntity:GetTransformation():GetPos(); --playerEnt:GetCollisionResponder():GetCollisionPosition(playerEnt);
	
	--local frontVec = Entity.GetEntityByNetworkID(userId, ReservedActionID.CONNECT, 1):GetTransformation():GetOrient():GetFront();
	local networkEnt = Network.New(self, userId, actionId);
	--Logging.Log(LogLevel.DEBUG_PRINT, "ColPos: ");
	local collisionComp = Collision.New(self);
	local colRespComp = CollisionResponder.New(self);
	local physicsComp = Physics.New(self);
	local transformComp = Transformation.New(self);
	local scriptComp = Script.New(self, "Explosion");
	local timerComp = Timer.New(self, Explosion.duration);
	collisionComp:CreateHandle(self, 1, true);
	transformComp:SetPos(posVec);
	colRespComp:SetContainer(collisionComp);
	--Logging.Log(LogLevel.DEBUG_PRINT, "Explosion handle: "..collisionComp:GetHandle());
	physicsComp:BindNoShape(collisionComp, posVec, Quat.New(0,0,0,1));
	--Logging.Log(LogLevel.DEBUG_PRINT, "After bind call");
	physicsComp:CheckRadius(collisionComp:GetHandle(), Vec3.New(posVec.x, posVec.y, posVec.z), 20);
	if Global.IsClient then
		local particleComp = ParticleEmitter.New(self, "explosion");
	end
	--Logging.Log(LogLevel.DEBUG_PRINT, "End of Oncreate");
end

function Explosion.OnCollide (self, entity)
	--Logging.Log(LogLevel.DEBUG_PRINT, "OnCollide");
	if entity:DoesExist() then
		local hitCol = entity:GetCollision();
		local hitPhys = entity:GetPhysics();
		local type = hitPhys:GetType(hitCol);
	 	if type == PhysicsType.TYPE_PLAYER then
		    local abilityOwnerNetwork = self:GetNetwork();
		    local abilityOwnerId = abilityOwnerNetwork:GetUserId();
		    local abilityOwnerEntity = Entity.GetEntityByNetworkID(abilityOwnerId, ReservedActionID.CONNECT, 0);
		   	local abilityOwnerPlayerComponent = abilityOwnerEntity:GetPlayerComponent();
		   	local targetPlayerComponent = entity:GetPlayerComponent();
		    if abilityOwnerPlayerComponent:GetTeamId() ~= targetPlayerComponent:GetTeamId() then
			    local network = entity:GetNetwork();
			    local hitPos = entity:GetTransformation():GetPos();
			    local selfPos = self:GetTransformation():GetPos();
			    hitPhys:KnockBack(hitCol:GetHandle(), Vec3.New(hitPos.x-selfPos.x,2,hitPos.z-selfPos.z), Explosion.pushback);
			    local health = entity:GetHealth();
			    if not health:IsDead() then
				    local receiverId = network:GetUserId();
				    health:Damage(abilityOwnerId, Explosion.damage, receiverId);
			    end
			end
		end
	end
end
function Explosion.OnDestroy (self)
	Entity.Remove(self);
end

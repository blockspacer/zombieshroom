Player = {}

function Player.OnCreate(userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating player (userId: "..tostring(userId)..", actionId: "..tostring(actionId)..")");
	
	local player = Entity.New();
	local playerComponent = PlayerComponent.New(player);
 	local network = Network.New(player, userId, actionId);
	
 	playerComponent:SetAbility(3, "Push", -1);
	playerComponent:SelectAbility(0);
 	playerComponent:SetDeaths(0);
	playerComponent:SetScore(0);
	playerComponent:SetTeamId(3);
  
	end
  
	if Global.UserID == userId then
		local playerControl = PlayerControl.New(player);


		playerControl:SetMouseSensitivity(0.2);


		Entity.RegisterTag("Player", player);
		Entity.RegisterTag("AimingDevice", aimingEntity);
	end
end

function Player.OnTeamSelect(self, teamId)
	local transform = Transformation.New(self);
	local playerPhysics = PlayerPhysics.New(self);
	local health = Health.New(self);
	local physics = Physics.New(self);
	local collision = Collision.New(self);
	local collisionResponder = CollisionResponder.New(self);
	local script = Script.New(self, "Player");
	local playerAction = PlayerAction.New(self);
	local stateComponent = StateComponent.New(self);
	local tryPickup = TryPickupComponent.New(self);
  
  	transform:SetPos(Vec3.New(100,10,0));

	stateComponent:SetPreviousPosition(transform:GetPos());
	stateComponent:SetCurrentState(EntityState.DESCENDING);

	playerAction:SetJumpTime(0.0);
	playerAction:SetMovePower(0);
	playerAction:SetStrafePower(0);
	playerAction:SetAngle(Vec2.New(0, 0));
	playerAction:SetAbilityTime(0.0);
	playerAction:SelectAbility(1);

	playerPhysics:SetMovementSpeed(25);
	playerPhysics:SetJumpForce(10); --Do not fucking change without good reason or I will hunt you down //Kim
	playerPhysics:SetJumpBoostForce(0.9); --See comment above //Kim

	collision:SetMeshHandle("testchar0");
	Collision.AddPlayerObjectToWorld(player, collision, transform, playerPhysics, collisionResponder);

	health:SetHealth(100);
	health:SetIsDead(false);
	health:SetWantsRespawn(false);


	Entity.RegisterGroup("NonExport", player);

	-- Create an aiming device
	local aimingEntity = Entity.New();
	local aimingTransform = Transformation.New(aimingEntity);
	local aimingNetwork = Network.New(aimingEntity, userId, actionId);

	Entity.RegisterGroup("NonExport", aimingEntity);
	
	if Global.IsClient then
		local renderable = Renderable.New(player);
		local animation = Animation.New(player);
		local ragdoll = Ragdoll.New(player);
		renderable:SetPass(RenderPass.RENDERPASS_DEFAULT);
		renderable:SetShadowTechnique(ShadowTechnique.SHADOW_ANIMATED);
		renderable:SetModel("testchar");
		if playerComponent:GetTeamId() == 1 then
			renderable:SetMaterial("BlueSpirit");
			renderable:SetMaterialGlow("WSGlowBlue");
		else
			renderable:SetMaterial("RedSpirit");
			renderable:SetMaterialGlow("WSGlowRed");
		end
		renderable:SetMaterialDiffuse("WStexture");
		renderable:SetMaterialSpecular("WSSpecular");
		renderable:SetMaterialNormal("WSNormal");
		renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
		renderable:SetAnimation(animation);
		local waterCollider = WaterCollider.New(player);
		waterCollider:SetDisturbPower(0.5);
		waterCollider:SetDisturbInterval(0.5);
		waterCollider:SetRadius(5);
end

function Player.OnCollide (self, entity)
	-- Logging.Log(LogLevel.DEBUG_PRINT, "Entity collided");
 	local hitCol = entity:GetCollision();
 	local hitPhys = entity:GetPhysics();
	local type = hitPhys:GetType(hitCol);
  if  type == PhysicsType.TYPE_ABILITYSPAWN then
    --Logging.Log(LogLevel.DEBUG_PRINT, "collided with ability spawn!");
    --local action = self:GetPlayerAction();
    --local playerComponent = self:GetPlayerComponent();
    --if action:TryPickup() then
    --  local abilitySpawn = entity:GetAbilitySpawn();
    --  playerComponent:SetAbility(playerComponent:GetSelectedAbility(), abilitySpawn:GetCurrentName(), abilitySpawn:GetCurrentCharges());
    --  action:SetTryPickup(false);
    --  abilitySpawn:SetClaimed(true);
    --  Logging.Log(LogLevel.DEBUG_PRINT, "pick up successful!");
    --end
  end
  
end

function Player.OnDestroy (self)
	Logging.Log(LogLevel.DEBUG_PRINT, "Entity destroyed");
	local collision = self:GetCollision();
	Collision.RemoveObjectFromWorld(collision);
end
Player = {}

function Player.OnCreate(userId, actionId)
	Logging.Log(LogLevel.DEBUG_PRINT, "Creating player");
	
	local player = Entity.New();
	local transform = Transformation.New(player);
	local playerPhysics = PlayerPhysics.New(player);
	local health = Health.New(player);
	local physics = Physics.New(player);
	local playerComponent = PlayerComponent.New(player);
	local collision = Collision.New(player);
	local collisionResponder = CollisionResponder.New(player);
	local script = Script.New(player, "Player");
	local playerAction = PlayerAction.New(player);
	local stateComponent = StateComponent.New(player);
	local network = Network.New(player, userId, actionId);
	local tryPickup = TryPickupComponent.New(player);
	--local waterCollider = WaterCollider.New(player);
	--waterCollider:SetDisturbPower(0.0);
	--waterCollider:SetDisturbInterval(0.5);
	--waterCollider:SetRadius(5);

	-- TODO: Decide where to put spawn logic
	transform:SetPos(Vec3.New(100,10,0));

	

	stateComponent:SetPreviousPosition(transform:GetPos());
	stateComponent:SetCurrentState(EntityState.DESCENDING);

	playerAction:SetJumpTime(0.0);
	playerAction:SetMovePower(0);
	playerAction:SetStrafePower(0);
	playerAction:SetAngle(Vec2.New(0, 0));
	playerAction:SetAbilityTime(0.0);
	playerAction:SelectAbility(1);

	playerComponent:SetAbility(0, "AbilityBall", -1);
	playerComponent:SetAbility(1, "AbilityDash", -1);
	playerComponent:SetAbility(2, "MagicMissile" ,-1);
	playerComponent:SelectAbility(0);

	playerPhysics:SetMovementSpeed(20);
	playerPhysics:SetJumpForce(20);

	collision:SetMeshHandle("testchar0");
	Collision.AddPlayerObjectToWorld(player, collision, transform, playerPhysics, collisionResponder);

	health:SetHealth(100);
	health:SetIsDead(false);
	health:SetWantsRespawn(false);
	playerComponent:SetDeaths(0);
	playerComponent:SetScore(0);
	-- ToDo: Get and set a correct team id
	playerComponent:SetTeamId(1 + userId - math.floor(userId/2) * 2);

	Entity.RegisterGroup("NonExport", player);

	-- Create an aiming device
	local aimingEntity = Entity.New();
	local aimingTransform = Transformation.New(aimingEntity);
	local aimingNetwork = Network.New(aimingEntity, userId, actionId);

	Entity.RegisterGroup("NonExport", aimingEntity);
	
	if Global.IsClient then
		local renderable = Renderable.New(player);
		local animation = Animation.New(player);
		--local ragdoll = Ragdoll.New(player);
		renderable:SetPass(RenderPass.RENDERPASS_DEFAULT);
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
	end
  
	if Global.UserID == userId then
		local playerControl = PlayerControl.New(player);

		playerControl:SetMouseSensitivity(0.2);

		Entity.RegisterTag("Player", player);
		Entity.RegisterTag("AimingDevice", aimingEntity);
	end
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
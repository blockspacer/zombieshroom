Player = {}

function Player.OnCreate(self, userId, actionId)
	Logging.Log("Hello", 0);
	local transform = Transformation.New(playerEntity);
	local playerPhysics = PlayerPhysics.New(playerEntity);
	local health = Health.New(playerEntity);
	local player = PlayerComponent.New(playerEntity);
	local physics = Physics.New(playerEntity);
	local collision = Collision.New(playerEntity);
	local collisionResponder = CollisionResponder.New(playerEntity);
	local script = Script.New(playerEntity, "Player");
	local playerAction = PlayerAction.New(playerEntity);
	local network = Network.New(playerEntity, userId, actionId);

	-- TODO: Decide where to put spawn logic
	player:SetAbility(0, "AbilityTest");
	player:SelectAbility(0);

	playerPhysics:SetMovementSpeed(10);
	playerPhysics:SetJumpForce(20);

	physics:SetMass(5);

	-- Create an aiming device
	local aimingEntity = Entity.New();
	local aimingTransform = Transformation.New(aimingEntity);
	local aimingNetwork = Network.New(aimingEntity, userId, actionId);
end

function Player.AddClientComponents(self)
	local renderable = Renderable.New(playerEntity);
	local animation = Animation.New(playerEntity);

	renderable:SetModel("testchar");
	renderable:SetMaterial("testchar");
	renderable:SetMaterialDiffuse("WStexture");
	renderable:SetMaterialSpecular("WSSpecular");
	renderable:SetMaterialNormal("WSNormal");
	renderable:SetMaterialEffect("Mesh_NormalMap_Anim");
	renderable:SetAnimation(animation);
end

function Player.AddLocalClientComponents(self)
	
end

function Player.OnCollide (self, entity)
	Logging.Log("Entity collided");
end

function Player.OnDestroy ()
	
end
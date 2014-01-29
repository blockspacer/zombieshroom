function AddMessage(p_message)
{
	$("#chatlog").html( $("#chatlog").html() + p_message + "<br>");
	$("#chatlog").scrollTop($("#chatlog")[0].scrollHeight);
}
function Send(event)
{
	$('#chatinput').blur();
	ChatSystem.Send($("#chatinput").val());
	$("#chatinput").val("");
	event.preventDefault();
	event.stopPropagation();
}
function SetDeathScreen(p_visible)
{
	$("#deathscreen").css("display", (p_visible?"block":"none"));
}

$(document).keydown(function(event)
{
	if(!$('#chatinput').is(":focus") && event.keyCode == 13) // Enter
	{
		$('#chatinput').focus();
	}
	else if(event.keyCode == 27) // Esc
	{
		$('#chatinput').blur();
	}
	else
		return true;
});

function OnLoad()
{
	$('#chatinput').blur(function(){ ChatSystem.SetFocus(false); });
	$('#chatinput').focus(function(){ ChatSystem.SetFocus(true); });
	$('#chatinput').keydown(function()
	{
		if(event.keyCode == 13)
			Send(event);
		else
			return true;
	});
}

function Set(p_id, p_value)
{
	var value = p_value;
	if(p_id == "TimeLeft")
	{
		var time = parseFloat(value);
		var minutes = Math.floor(time / 60);
		var seconds = time - (minutes * 60);
		if(minutes < 10)
			minutes = "0" + minutes;
		if(seconds < 10)
			seconds = "0" + seconds;
		value = "" + minutes + ":" + seconds;
		if( minutes < 2 )
			$("#"+p_id).addClass("blink");
		else
			$("#"+p_id).removeClass("blink");
	}
	if(p_id == "Health")
	{
		SetDeathScreen(parseInt(value) <= 0);
	}
	$("#"+p_id).html(value);
}

function SetAbility(p_slot, p_ability)
{
	$("#slot"+p_slot).css("background-image", "url('"+p_ability+".png')");
}

function SetAbilityFocus(p_slot)
{
	$(".slotselected").removeClass("slotselected");
	$("#slot"+p_slot).addClass("slotselected");
}
function StartCooldown(p_slot, p_duration)
{
	var slot = $("#slot"+p_slot+"-cooldown");
	if(parseInt(slot.css("background-position-y").split("px")[0]) == 75)
	{
		slot.css("background-position", "50% 0px");
		slot.animate({'background-position-y': '75px'}, p_duration*1000, 'linear');
	}
}

// $(document).ready(function(){
	// SetAbilityFocus(0);
	// SetAbility(1, "TestBall");
	// StartCooldown(1, 3);
// });

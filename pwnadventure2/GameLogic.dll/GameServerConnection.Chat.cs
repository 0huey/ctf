public void Chat(string text)
{
	if (text.ToLower().StartsWith("pos"))
	{
		LocalPlayerEvents.ChatMessage(LocalPlayerEvents.localPlayer.transform.position.ToString());
		return;
	}
	if (text.ToLower().StartsWith("tp"))
	{
		string[] array = text.Split(new char[]{' '});
		if (array.Length > 3)
		{
			Vector3 pos;
			pos.z = float.Parse(array[array.Length - 1]);
			pos.y = float.Parse(array[array.Length - 2]);
			pos.x = float.Parse(array[array.Length - 3]);
			LocalPlayerEvents.localPlayer.transform.position = pos;
		}
		return;
	}
	if (text.ToLower().StartsWith("fly"))
	{
		if (LocalPlayerEvents.localPlayer.flyingEnabled)
		{
			LocalPlayerEvents.localPlayer.flyingEnabled = false;
			Physics.gravity = new Vector3(0f, -9.81f, 0f);

		}
		else
		{
			LocalPlayerEvents.localPlayer.flyingEnabled = true;
			Physics.gravity = new Vector3(0f, 0f, 0f);
		}
		return;
	}
	/*
	if (text.ToLower().StartsWith("map"))
	{
		String map = text.Split(new char[]{' '})[-1].ToLower();
		if (map.Contains("town")) {

		}
		return;
	}
	*/
	object obj = this.commandQueue;
	lock (obj)
	{
		this.commandQueue.Enqueue(delegate
		{
			try
			{
				GameServerMessage gameServerMessage = new GameServerMessage(GameServerMessage.Command.ChatCommand);
				gameServerMessage.GetWriter().Write(text);
				gameServerMessage.Serialize(this.stream);
				this.bytesSent += gameServerMessage.length;
			}
			catch (Exception)
			{
				this.Stop();
			}
		});
		this.commandEvent.Set();
	}
	return;
}

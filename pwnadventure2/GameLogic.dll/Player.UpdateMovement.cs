public void UpdateMovement(Vector2 movementControls, float slope, bool flying, bool running, bool sneaking)
{
	float num;
	float num2;
	float d;
	if (sneaking)
	{
		num = ((!flying) ? 1f : 0f);
		num2 = 0.35f;
		d = 0.0084f;
	}
	else if (running)
	{
		num = ((!flying) ? 1f : 0.9f);
		num2 = 0.15f;
		d = 0.1f;
	}
	else
	{
		num = ((!flying) ? 1f : 0.35f);
		num2 = 0.17f;
		d = 0.018f;
	}
	this.throttle += movementControls.y * (base.transform.TransformDirection(Vector3.forward * d * num) * slope);
	this.throttle += movementControls.x * (base.transform.TransformDirection(Vector3.right * d * num) * slope);
	if (this.flyingEnabled)
	{
		float vAngle = this.remoteLookDirection.eulerAngles.x;
		if (vAngle <= 90f)
		{
			vAngle *= -1f;
		}
		else
		{
			vAngle = 90f - vAngle % 90f;
		}
		this.throttle += movementControls.y * (base.transform.TransformDirection(Vector3.up * d * num) * (float)Math.Sin((double)vAngle * 3.141592653589793 / 180.0));
	}
	this.throttle.x = this.throttle.x / (1f + num2 * num);
	this.throttle.y = this.throttle.y / (1f + num2 * num);
	this.throttle.z = this.throttle.z / (1f + num2 * num);
	this.throttle.x = ((Mathf.Abs(this.throttle.x) >= 0.0001f) ? this.throttle.x : 0f);
	this.throttle.y = ((Mathf.Abs(this.throttle.y) >= 0.0001f) ? this.throttle.y : 0f);
	this.throttle.z = ((Mathf.Abs(this.throttle.z) >= 0.0001f) ? this.throttle.z : 0f);
}

/* Update vruntime everytime because the process was given cpu time */
curr->vruntime += calc_delta_fair(delta_exec, curr);

/* Update soft_realtime_requirements on context switch if it is not 0 */
u64 delta_fair_exec = calc_delta_fair(delta_exec, curr);

if (curr->soft_realtime_nice != 0)
{
	if (curr->soft_realtime_nice < delta_fair_exec)
	{
		curr->soft_realtime_nice -= delta_fair_exec;
	}
	else
	{
		curr->soft_realtime_nice = 0;
	}
}
# Reference

[cern introduction of HTCondor for lxplus](http://batchdocs.web.cern.ch/batchdocs/index.html)
# Authentication

# Before start

**klist** : to see the valid kerberos credentials

**condor\_q** : to see the job status

If you fail to see the proper output of these command, use **kinit** to refresh tokens as necessary.

The system will remove automatically the running jobs if they are in running state for more than the MaxRuntime or JobFlavour.

The system will remove automatically the held jobs if they are in hold state for more than 24 hours.

The system will remove automatically the jobs which restarted for more than 10 times.

# Quick Start Quide

## Submit file

An example submit file:

```
executable            = hello_world.sh
arguments             = $(ClusterID) $(ProcId)
output                = output/hello.$(ClusterId).$(ProcId).out
error                 = error/hello.$(ClusterId).$(ProcId).err
log                   = log/hello.$(ClusterId).log
queue
```

 * executable: The script or command you want HTCondor to run.
 * arguments: Any arguments that could be passed to the command. We're using interpolated values here. ClusterId will normally be unique to each submit file. There are circumstances in which one submit file can create multiple clusters, but let's ignore that for now. ProcId is incremented by one for each job in each cluster. In this simple example, which is defining a single job, the value of ProcId will be 0.
 * output: where the STDOUT of the command or script should be written to. This can be a relative or absolute path. HTCondor won't create the directory for you though, and will error if it doesn't exist. Note the use of interpolation again to split up the output.
 * error: where the STDERR of the command or script would be written to. Same rules apply as output.
 * log: This is the output of HTCondor's logs for your jobs, not any logging your job itself will perform. It will show the submission times, execution host and times, and on termination will show stats.
 * queue: This schedules the job. It becomes more important (along with the interpolation) when queue is used to schedule multiple jobs by taking an integer as a value.

## Submitting the job

 On lxplus (or another configured submit host) you simply need to run the condor\_submit command:

 ```
 $ condor_submit hello.sub
 Submitting job(s).
 1 job(s) submitted to cluster 70.
 ```

 You will note the reference again to "cluster", this output shows the "ClusterId" referred to in the submit file. Normally, you get one cluster per run of condor\_submit.

## Monitoring the job

 ```
 $ condor_q
 OWNER   BATCH_NAME       SUBMITTED   DONE   RUN    IDLE  TOTAL JOB_IDS
 bejones CMD: hello.sh  10/3  14:08      _      _      1      _ 70.0
 ```

 Note the job id "70.0" showing the **cluster id (70)** and the **process id (0)**. We only have one subjob in this submit file, so we only have job id 0.

 Rather than monitoring the job using repeated invocations of condor\_q, you can use condor\_wait:
 ```
 $ condor_wait -status log/hello.70.log
 ```



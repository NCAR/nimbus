/**
 * Some testing code for filterCounter() in filter.c
 */
float HighRateData[50], sdp[250];

#include <cstdio>


// Check to see if we passed the test.
void checkHRT(int outRate, int sampleRate)
{
  float inSum = 0.0, outSum = 0.0;

  for (int i = 0; i < sampleRate; ++i)
    inSum += sdp[i];

  for (int i = 0; i < outRate; ++i)
    outSum += HighRateData[i];

//  if (inSum != outSum)
    fprintf(stderr, "%d %d test, res = %f %f\n", outRate, sampleRate,
	inSum, outSum);
}

// filter some data.
void runTest(int outRate, int sampleRate)
{
  int        OUTindex;

  int L = outRate / sampleRate;
  int M = sampleRate / outRate;
/*
  if (M != 0 && (float)M != (float)outRate / sampleRate)
    fprintf(stderr, "M not even rate\n");

  if (L != 0 && (float)L != (float)sampleRate / outRate)
    fprintf(stderr, "L not even rate\n");
*/
  printf("L=%d, M=%d\n", L, M);

  if (L > 0)
    for (OUTindex = 0; OUTindex < outRate; ++OUTindex)
      HighRateData[OUTindex] = sdp[OUTindex / L] / L;

  if (M > 0)
    for (OUTindex = 0; OUTindex < outRate; ++OUTindex)
    {
      float sum = 0.0;
      for (int idx = 0; idx < M; ++idx)
        sum += sdp[(OUTindex*M)+idx];

      HighRateData[OUTindex] = sum;
    }

  checkHRT(outRate, sampleRate);
}


void init_inData()
{
  for (int i = 0; i < 250; ++i)
    sdp[i] = i * 2 + 10;
}

int main()
{
  init_inData();
  runTest(25, 1);
  runTest(25, 5);
  runTest(25, 50);
  runTest(25, 250);
  runTest(50, 1);
  runTest(50, 25);
  runTest(50, 100);
  return 0;
}

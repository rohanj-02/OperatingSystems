// size_t NUMBER_OF_ARGUMENTS = 10;

// char **getInput(char **inputArr)
// {
// 	char x;
// 	int counter = 0;
// 	int smallCounter = 0;
// 	// int CURRENT_SIZE = sizeof(inputArr) / sizeof(char);
// 	// printf("%d", NUMBER_OF_ARGUMENTS);
// 	// size_t NUMBER_OF_ARGUMENTS = 5;
// 	bool flag = true;
// 	while (x = getchar())
// 	{
// 		if (x == '\n')
// 		{
// 			break;
// 		}
// 		else if (x == 32)
// 		{
// 			inputArr[counter][smallCounter] = '\0';
// 			smallCounter = 0;
// 			counter++;
// 			if (counter >= NUMBER_OF_ARGUMENTS)
// 			{
// 				int newSize = 2 * NUMBER_OF_ARGUMENTS;
// 				newSize = newSize * sizeof(char *);
// 				char **newp = (char **)realloc(inputArr, newSize);
// 				inputArr = newp;
// 				NUMBER_OF_ARGUMENTS *= 2;
// 			}
// 			flag = true;
// 		}
// 		else
// 		{
// 			if (flag)
// 			{
// 				flag = false;
// 				char temp[2] = "\0";
// 				temp[0] = x;
// 				inputArr[counter][smallCounter] = x;
// 				// strcpy(inputArr[counter], temp);
// 			}
// 			else
// 			{
// 				// inputArr[counter] += x;
// 				char str[2] = "\0";
// 				str[0] = x;
// 				inputArr[counter][smallCounter] = x;
// 				// strcat(inputArr[counter], str);
// 			}
// 			smallCounter++;
// 			// strcpy(inputArr[counter], inputArr[counter] + x);
// 		}
// 	}
// 	// printf("Loop completed bro!");
// 	return inputArr;
// }

// MAIN
// system("clear");
// int initialSize = NUMBER_OF_ARGUMENTS * sizeof(char *);
// char **x = (char **)malloc(initialSize);
// for (int i = 0; i < initialSize; i++)
// {
// 	x[i] = (char *)malloc(100);
// }
// x = getInput(x);
// printf("%ld", NUMBER_OF_ARGUMENTS);
// for (int i = 0; i < NUMBER_OF_ARGUMENTS; i++)
// {
// 	printf("%s\n", x[i]);
// }
// free(x);
// char **input = (char **)calloc(10, sizeof(char *));
// for (int i = 0; i < 10; i++)
// {
// 	// input[i] = (char *)calloc(10, 1);
// 	// scanf("%s", input[i]);
// 	input[i] = getOneArgument();
// 	// strcpy(input[i], "YOLO");
// 	// printf("Pointer:%p Value:%s Length:%ld \n", *input[i], input[i], strlen(input[i]));
// }
// char **newptr = (char **)realloc(input, 20 * sizeof(char *));
// input = newptr;
// for (int i = 10; i < 20; i++)
// {
// 	input[i] = (char *)calloc(10, 1);
// 	strcpy(input[i], "SECOND?");
// }
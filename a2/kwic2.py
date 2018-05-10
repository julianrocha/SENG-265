#!/usr/bin/env python3

#kwic2.py by Julian Rocha, V00870460, March 7th 2018
import sys
import copy
from collections import defaultdict

#Global Constants
NUM_LINE_BEFORE_EX = 2	#Number of lines in input before exlcusion words begin (2, ::)
NUM_SPACE_BEFORE = 9	#Number of spaces before printing begins
KEY_START = 20			#Number of chars to print before keyword begins
LEN_AFTER_KEY = 30		#Number of chars to print after start of keywords

#Read from stdin, return a list of lines without \n
def readIn():
	return [string.rstrip() for string in sys.stdin.readlines()]

#Read from inputText, return a list of exlusion words (lowercase)
def grabExlusionWords(inputText):
	secondColon = inputText[NUM_LINE_BEFORE_EX:].index('::') + NUM_LINE_BEFORE_EX
	return [word.lower() for word in inputText[NUM_LINE_BEFORE_EX:secondColon]]

#Read from inputText, return a list of phrases (without empyt lines)
def grabLines(inputText):
	secondColon = inputText[NUM_LINE_BEFORE_EX:].index('::') + NUM_LINE_BEFORE_EX + 1
	return [line for line in inputText[secondColon:] if line != '']

#Read from lines, return the same list in all lowercase 
def linesToLower(lines):
	return [line.lower() for line in lines]

#Read from linesLower, return a 2D list (lines x words), without exlusion words
def grabKeyWords(linesLower, exWords):
	keyWords = [string.split(' ') for string in linesLower]
	lineCounter = 0
	for line in keyWords:
		keyWords[lineCounter] = [word for word in line if word not in exWords]
		lineCounter += 1
	return keyWords

#Read from keyWords, return a dictionary (keyLineDict)
#(key = keyword, value = list of the line index's of keywords)
def grabKeyWordLineNums(keyWords):
	keyLineDict = defaultdict(list)		#dictionary of lists
	lineCounter = 0
	for line in keyWords:
		for word in line:
			keyLineDict[word].append(lineCounter)
		lineCounter += 1
	return keyLineDict

#Read from linesLower, determine the column index of each keyword
#Store the column index's in a dictionary (colNumDict)
#(key = keyword, value = list of column index's of a keyword in a line from lines)
def grabKeywordColumn(keyLineDict, sortedKeyWords, linesLower):
	colNumDict = copy.deepcopy(keyLineDict)
	for word in sortedKeyWords:
		occurence = 0
		for lineNum in keyLineDict[word]:
			#if the keyword is in the middle of the line
			if (" " + word + " ") in linesLower[lineNum]:
				colNumDict[word][occurence] = linesLower[lineNum].index(" " + word + " ") + 1
			#if the keyword is at the end of the line
			elif (" " + word) in linesLower[lineNum]:
				if linesLower[lineNum].index(" " + word) + len(word) + 1 == len(linesLower[lineNum]):
					colNumDict[word][occurence] = linesLower[lineNum].index(" " + word) + 1
			#if the keyword is at the start of the line
			elif (word + " ") in linesLower[lineNum]:
				if linesLower[lineNum].index(word + " ") == 0:
					colNumDict[word][occurence] = 0
			#if the keyword is the only word in the line
			elif (word) in linesLower[lineNum]:
				if linesLower[lineNum].index(word) == 0:
					colNumDict[word][occurence] = 0
			occurence += 1
	return colNumDict

#Iterates through sortedKeyWords and lines and formats an output
#Makes use of keyLineDict and colNumDict to search through lines
#Stores output in outputText
def formatOutput(outputText, sortedKeyWords, lines, keyLineDict, colNumDict):
	
	outputLine = 0							#int to keep track of what line of output to add to
	for word in sortedKeyWords:				#Loop though keywords
		occurence = 0						#int to keep track of what occurence of keyword is being used
		for lineNum in keyLineDict[word]:	#Loop through the lines a keyword is from

			keyCol = colNumDict[word][occurence]	#Column location of keyword (first letter)
			lineStart = 0	#Line position to start printing from

			
			if keyCol > KEY_START:	#Adjust start position when keyword starts too late 
				lineStart = keyCol - KEY_START
			if keyCol < KEY_START: #Add spaces when keyword starts too early
				outputText[outputLine] += (' ' * (KEY_START - keyCol))

			if lineStart > 0:	#Shift line start right if a word has been cut off
				while(lines[lineNum][lineStart - 1] != ' '):
					outputText[outputLine] += ' '
					lineStart += 1

			
			keyLen = len(word)				#Length of keyword
			lineStart2 = keyCol + keyLen 	#Line position to start printing after word
			lineEnd = len(lines[lineNum])	#Line position to end printing
			lineLen = lineEnd - lineStart2 + keyLen #Length of line after word
			

			if lineLen > LEN_AFTER_KEY:			#Adjust lineEnd if line length is too long
				lineEnd = keyCol + LEN_AFTER_KEY
			if lineEnd < len(lines[lineNum]) - 1:	#Shift lineEnd left if it cuts off a word
				while(lines[lineNum][lineEnd + 1] != ' '):
					lineEnd -= 1


			outputText[outputLine] += lines[lineNum][lineStart:keyCol]	#Insert start line
			outputText[outputLine] += word.upper()						#Insert Caps keyword
			outputText[outputLine] += lines[lineNum][lineStart2:lineEnd + 1]#Insert line end
			
			occurence += 1	#Increment to next occurence of keyword
			outputLine += 1	#Increment to next line of output
	return

#Prints outputText
def printOutput(outputText):
	for line in outputText:
		print(line)
	return

def main():
	inputText = readIn()
	exWords = grabExlusionWords(inputText)
	lines = grabLines(inputText)
	linesLower = linesToLower(lines)
	keyWords = grabKeyWords(linesLower,exWords)
	keyLineDict = grabKeyWordLineNums(keyWords)
	sortedKeyWords = sorted(keyLineDict)
	colNumDict = grabKeywordColumn(keyLineDict, sortedKeyWords, linesLower)

	numKeys = 0			#Number of keywords (including occurences)
	for word in sortedKeyWords:
		numKeys += len(keyLineDict[word])
	outputText = [' ' * NUM_SPACE_BEFORE] * numKeys

	formatOutput(outputText, sortedKeyWords, lines, keyLineDict, colNumDict)
	printOutput(outputText)

if __name__ == '__main__':
	main()
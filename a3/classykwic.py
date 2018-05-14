#By Julian Rocha, V00870460, March 22nd 2018
import re
class Kwic:
	KEY_START = 20			#Number of chars to print before keyword begins
	LEN_AFTER_KEY = 30		#Number of chars to print after start of keywords
	#Constructor, initializes 4 variables, calls 2 functions
	def __init__(self, excluded, lines):
		self.excluded = excluded[:]			#list of lowercase exclusion words
		self.index_lines = lines[:]			#list of passed index_lines, no empty lines
		self.keywords = self.grabKeywords()	#list of lowercase keywords from index_lines, no repeats
		self.key_lines = self.keyLines()	#list of formatted keywords in lines (output)
	
	#Find and sort lowercase keywords from index_lines into a list
	def grabKeywords(self):
		keywords = []						#empty list to add lowercase keywords to
		for line in self.index_lines:
			for word in line.lower().split(' '):	
				if word not in self.excluded and word not in keywords:
					keywords.append(word)
		keywords.sort()
		return keywords
	
	#Return a list of formatted lines of each capatalized keyword
	def keyLines(self):
		key_lines = []									#empty list to add formatted lines
		for word in self.keywords:
			pattObj = re.compile('(\\b.{,'+str(self.KEY_START-1)+'} |^)'+word+'( .{,'+str(self.LEN_AFTER_KEY-len(word))+'}\\b|$)', re.IGNORECASE)
			# ^ make a formatted group to the left of keyword and a formatted group to the right of keyword
			for line in self.index_lines:
				lineMatch = pattObj.search(line)		#create match object for line and keyword
				if lineMatch:							#if keyword was found in line
					key_lines.append((self.LEN_AFTER_KEY - 1 -len(lineMatch.group(1)))*" "+lineMatch.group(1)+word.upper()+lineMatch.group(2).rstrip())
					# ^ append the appropriate num spaces before (determined by global variable and length of group 1, 
					#then append group 1, the uppercase word, then group 2 with whitespace stripped
		return key_lines
	
	#Return formatted lines
	def output(self):
		return self.key_lines
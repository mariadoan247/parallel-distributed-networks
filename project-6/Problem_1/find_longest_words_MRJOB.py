from mrjob.job import MRJob
import re


class MRLongestWord(MRJob):

    def mapper(self, _, line):
        line = line.lower()
        line = re.sub(r'[^a-z]', " ", line)
        words = line.split()
        for word in words:
            yield word[0], word

    def reducer(self, key, values):
        longestWords = []
        longestLen   = 0
        for word in values:    
            if longestLen < len(word):
                longestWords = [word]
                longestLen = len(word)
            elif (longestLen == len(word) and longestWords.count(word) == 0):
                longestWords += [word]
        yield key, longestWords


if __name__ == '__main__':
    MRLongestWord.run()

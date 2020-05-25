# SubCipherImproved
 A improved version of my substitution decipher from my programming assignment
 This works by using the hill-climbing algorithim which is:
  1. Generate a random key, called the 'parent', decipher the ciphertext using this key. Rate the fitness of the deciphered text, store the result.
  2. Change the key slightly (swap two characters in the key at random), measure the fitness of the deciphered text using the new key. 
  3. If the fitness is higher with the modified key, discard our old parent and store the modified key as the new parent.
  4. Go back to 2, unless no improvement in fitness occurred in the last 1000 iterations.
  
 This version uses hashing which makes the need for sorting and searching irrevelant. All given quadgrams from the english_quadgrams.txt are hashed using a struct which contains their key value pair
  
 For more information go [here](http://practicalcryptography.com/cryptanalysis/stochastic-searching/cryptanalysis-simple-substitution-cipher) and [here](http://practicalcryptography.com/cryptanalysis/text-characterisation/quadgrams/#a-python-implementation)

/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Alan Li
 * yli346@jh.edu
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "apint.h"


/* Initiate a ApInt instance by taking an unsigned 64-bits value.Returns a pointer to an ApInt 
 * instance whose value is specified by the val parameter*/
ApInt *apint_create_from_u64(uint64_t val) {
  ApInt *ap = malloc(sizeof(ApInt));
  if(ap == NULL){
    return NULL;
  }
  ap->size = 1;
  ap->data = malloc(sizeof(uint64_t));
  if(ap->data == NULL){
    return NULL;
  }
  //assign value
  ap->data[0] = val;
  return ap;
}

/* Initiate a ApInt object by taking an constant char pointer hex, which is an arbitrary sequence
 * of hexadecimal (base 16) digits. Returns a pointer to an ApInt instance whose value is specified by the hex*/
ApInt *apint_create_from_hex(const char *hex) {
  size_t totalLen = strlen(hex);
  if(totalLen==0){
    return NULL;
  }
  //skip leading zeros
  size_t zeros = 0;
  for(size_t i = 0; i<totalLen; i++){
    if(hex[i]=='0'){
      zeros++;
    }
    else{
      break;
    }
  }

  if(zeros == totalLen){
    zeros = totalLen-1;
  }
  //set char to lower case
  char newHex[totalLen-zeros];
  for(size_t i = zeros; i<totalLen; i++){
    newHex[i-zeros] = tolower(hex[i]);
  }
  totalLen = totalLen - zeros;
  /*get the size of ApInt*/
  size_t aplen = totalLen%16 == 0? totalLen/16:totalLen/16+1;
  ApInt *ap = malloc(sizeof(ApInt));
  if(ap == NULL){
    return NULL;
  }
  ap->data = malloc(sizeof(uint64_t)*aplen);
  if(ap->data == NULL){
    return NULL;
  }
  ap->size = aplen;
  /* read the hex char by char and convert it to unsigned int in 64 bits */
  for(size_t i = 0; i<aplen; i++){
    uint64_t num = 0;
    int currChar = 16;
    if(i==aplen-1&&totalLen%16!=0){
      currChar = (int)totalLen%16;
    }
    for(size_t j = totalLen-i*16-currChar; j<=totalLen-i*16-1; j++){
      uint64_t byte = 0;
      if(isdigit(newHex[j])){
  	byte = newHex[j] - '0';
      }
      else{
  	byte = newHex[j]-'a'+10;
      }
      num = num*16+byte;
    }
    ap->data[i] = num;
    
  }
  
  return ap;
}

/* Deallocates the memory used by the ApInt instance pointed-to by the ap parameter. */
void apint_destroy(ApInt *ap) {
  free(ap->data);
  free(ap);
}

/* Returns a uint64_t value containing 64 bits of the binary representation of the ApInt instance pointed
 * to by ap. The parameter n indicates which bits to return. If n is 0, bits 0..63 are returned, if n is 
 * 1 bits 64..127 are returned, etc. The function should be prepared to handle arbitrarily large values of n. */
uint64_t apint_get_bits(ApInt *ap, unsigned n) {
  //return ap->data[n] is there is, elso return 0
  if(n<ap->size){
    return ap->data[n];
  }
  return 0;
}

/* Returns the position of the most significant bit set to 1 in representation of the ApInt pointed 
 *to by ap. As a special case, returns -1 if the ApInt instance pointed to by ap represents the value 0. */
int apint_highest_bit_set(ApInt *ap) {
  
  int res = 0;
  /* return -1 if the value of ap is 0*/
  if(ap->size==1&&ap->data[0]==0){
    return -1;
  }

  res+=(ap->size-1)*64;
  /*go to the highest bits set*/
  uint64_t count = ap->data[ap->size-1];
  while (count != 0) {
    count = count>>1;
    res++;
  }
  return res-1;
}

/* Returns a pointer to an ApInt instance formed by shifting each bit of the ApInt instance pointed to
 * by ap one bit position to the left. */

ApInt *apint_lshift(ApInt *ap) {
  ApInt *newAp = malloc(sizeof(ApInt));
  //if ap=0
  if(ap->size ==1 && ap->data[0]==0){
    newAp->data = malloc(sizeof(uint64_t));
    newAp->size = 1;
    newAp->data[0] = 0;
    return newAp;
   }
  //find the size
  int highest = apint_highest_bit_set(ap);
  if(highest%64==63){
    newAp->data = malloc(sizeof(uint64_t)*(ap->size+1));
    newAp->size = ap->size+1;
    //move with carry digit
    uint64_t carry = 0;
    for(size_t i = 0; i<newAp->size;i++){
      newAp->data[i] = (ap->data[i]<<1) +carry;
      if(ap->data[i]>>63==1){
	carry = 1;
      }
      else{
	carry = 0;
      }
    }
    newAp->data[ap->size] = 1;
    return newAp;
  }
  else{
    newAp->data = malloc(sizeof(uint64_t)*(ap->size));
    newAp->size = ap->size;
    uint64_t carry = 0;
    for(size_t i = 0; i<newAp->size;i++){
      newAp->data[i] = (ap->data[i]<<1) +carry;
      if(ap->data[i]>>63==1){
	carry = 1;
      }
      else{
	carry = 0;
      }
    }
    return newAp;
  }
}

/* Returns a pointer to an ApInt instance formed by shifting each bit of the ApInt instance pointed to
 * by ap n bit positions to the left.  */

ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
  ApInt *newAp = malloc(sizeof(ApInt));
  //if ap = 0
  if(ap->size ==1 && ap->data[0]==0){
    newAp->data = malloc(sizeof(uint64_t));
    newAp->size = 1;
    newAp->data[0] = 0;
    return newAp;
   }
  //find the size of ap
  size_t quotient = n/64;
  size_t remainder = n%64;
  if (remainder != 0){
    newAp->size = (ap->data[ap->size-1]>>(64-remainder)==0)?ap->size+quotient:ap->size+quotient+1;
  }
  newAp->data = malloc(sizeof(uint64_t)*newAp->size);
  
  for(size_t i = 0; i<quotient; i++){
    newAp->data[i] = 0;
  }
  //shift n%64 bit
  for(size_t i = 0; i<ap->size; i++){
    if(i == 0){
      newAp->data[quotient+i] = ap->data[i]<<remainder;
    }
    else{
      newAp->data[quotient+i] = (ap->data[i]<<remainder)+(ap->data[i-1]>>(64-remainder));
    }
  }
  if(newAp->size==(ap->size+quotient+1)){
    newAp->data[newAp->size-1] = ap->data[ap->size-1]>>(64-remainder);
  }

  return newAp;
}

char *apint_format_as_hex(ApInt *ap) {
  if(ap==NULL||ap->data==NULL){
    return NULL;
  }
  // if ap = 0
  if(ap->size ==1 && ap->data[0]==0){
    char *res = malloc(sizeof(char)*2);
    res[0] = '0';
    res[1] = '\0';
    return res;
  }
  else{
    size_t minSize = (ap->size-1)*16;
    uint64_t lastInt = ap->data[ap->size-1];
    while(lastInt!=0){
      lastInt = lastInt>>4;
      minSize++;
    }
    char *res = malloc((minSize+1)*sizeof(char));
    res[minSize] = '\0';
    minSize--;
    //produce uint64_t byte by byte
    for(size_t i = 0; i<ap->size; i++){
      uint64_t curr = ap->data[i];
      if(i!=ap->size-1){
  	uint64_t mask = 15;
  	for(int j = 0; j<16; j++){
  	  int bit = curr&mask;
  	  if(bit<=9){
  	    res[minSize] = '0'+bit;
  	  }
  	  else{
  	    res[minSize] = 'a'+bit-10;
  	  }
  	  minSize--;
  	  curr=curr>>4;
  	}
      }
      else{
  	while(curr!=0){
  	  uint64_t mask = 15;
  	  int bit = curr&mask;
  	  if(bit<=9){
  	    res[minSize] = '0'+bit;
  	  }
  	  else{
  	    res[minSize] = 'a'+bit-10;
  	  }
  	  minSize--;
  	  curr=curr>>4;
  	}
      }
    }
    return res;
  }
}

/* Computes the sum a plus b, and returns a pointer to an ApInt instance representing the sum. */
ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt *ap = malloc(sizeof(ApInt));
  const ApInt *larger = a;
  const ApInt *smaller = b;
  if(a->size<b->size){
    larger = b;
    smaller = a;
  }
  ap->data = malloc(sizeof(uint64_t)*larger->size);
  ap->size = larger->size;

  uint64_t carry = 0;
  //overlapped part addition
  for(size_t i = 0; i<smaller->size; i++){
    ap->data[i] = smaller->data[i]+larger->data[i]+carry ;
    if(ap->data[i]<smaller->data[i]||ap->data[i]<larger->data[i]){
      carry = 1;
    }
    else{
      carry = 0;
    }
  }
  //non-overlapped part addition
  for(size_t i = smaller->size; i<larger->size; i++){
    ap->data[i] = larger->data[i]+carry ;
    if(ap->data[i]<larger->data[i]){
      carry = 1;
    }
    else{
      carry = 0;
    }
  }
  if(carry ==1){
    ap->size++;
    ap->data = realloc(ap->data, ap->size*sizeof(uint64_t));
    ap->data[ap->size-1] = 1;
  }
  return ap;

}


/* Computes the difference a minus b, and returns a pointer to an ApInt instance representing the difference. As a special case, if b is greater than a, returns NULL (since the ApInt data type cannot represent a negative value.) */
ApInt *apint_sub(const ApInt *a, const ApInt *b) {
  if(apint_compare(a,b)<0){
    return NULL;
  }
  if(apint_compare(a,b)==0){
    return apint_create_from_u64(0);
  }
  ApInt *ap = malloc(sizeof(ApInt));
  ap->data = malloc(sizeof(uint64_t)*a->size);
  uint64_t borrow = 0;
  //overlapped part sub
  for(size_t i = 0; i<b->size; i++){
    uint64_t currA = a->data[i];
    uint64_t currB = b->data[i];
    ap->data[i] = currA-currB-borrow;
    if(currA-borrow<currB||(currA==0&&borrow==1)){
      borrow = 1;
    }
    else{
      borrow = 0;
    }
  }
  //non-overlapped part sub
  for(size_t i = b->size; i<a->size; i++){
    ap->data[i]= a->data[i]-borrow;
    if(a->data[i]==0&&borrow==1){
      borrow = 1;
    }
    else{
      borrow = 0;
    }
  }
  //realloc for leading zero
  for(int i = (int)a->size-1; i>=0; i--){
    if(ap->data[i]!=0){
      ap->size = i+1;
      ap->data = realloc(ap->data,(i+1)*sizeof(uint64_t));
      return ap;
    }
  }
  ap->size = 1;
  ap->data = realloc(ap->data,sizeof(uint64_t));
  ap->data[0] = 0;
  return ap;
}


/* Compares the values represented by the ApInt instances pointed-to by the parameters left and right. Returns a negative value if left is less that right, a positive value if left is greater than right, and 0 if the values are equal. */
int apint_compare(const ApInt *left, const ApInt *right) {
  //compare their size
  if(left->size<right->size){
    return -1;
  }
  else if(left->size>right->size){
    return 1;
  }
  else{
    //compare largest uint_t
    uint64_t l = left->data[left->size-1];
    uint64_t r = right->data[right->size-1];
    if(l<r){
      return -1;
    }
    else if(l>r){
      return 1;
    }
    else{
      return 0;
    }
  }
}

function shuffle(arr) {
  return arr.sort(() => Math.random() - 0.5);

}

function cipher(plaintext, substitutionKeyArr) {
  let result = '';
  [...plaintext].forEach(element => {
    result += substitutionKeyArr[element.charCodeAt() - 'a'.charCodeAt()];
  });
  return result;
}

const initailValue = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'w', 'v', 'x', 'y', 'z'];

const keyArr = shuffle(initailValue);
console.log('shuffled values:', keyArr);

console.log(cipher("attack", keyArr));
console.log(cipher("retreat", keyArr));
console.log(cipher("hold current position", keyArr));

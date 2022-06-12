import {writeFileSync, readFileSync} from 'fs';

type Instructions = 'STA' | 'ADD' | 'LDA' | 'HLT';

type Instruction = {
  instruction?: Instructions;
  argument?: string | number;
  indirect: boolean;
};

const tryInt = (value: string): string | number => isNaN(parseInt(value)) ? value : parseInt(value);

const InstructionTypes = {
  I   : 0b1000000000000000,
  AND : 0b0000000000000000,
  ADD : 0b0001000000000000,
  LDA : 0b0010000000000000,
  STA : 0b0011000000000000,
  BUN : 0b0100000000000000,
  BSA : 0b0101000000000000,
  ISZ : 0b0110000000000000,
  REG : 0b0111000000000000,
  CLA : 0b0111100000000000,
  CLE : 0b0111010000000000,
  CMA : 0b0111001000000000,
  CME : 0b0111000100000000,
  CIR : 0b0111000010000000,
  CIL : 0b0111000001000000,
  INC : 0b0111000000100000,
  SPA : 0b0111000000010000,
  SNA : 0b0111000000001000,
  SZA : 0b0111000000000100,
  SZE : 0b0111000000000010,
  HLT : 0b0111000000000001,
}

function run() {
  let words: string[]
    = readFileSync('./input/program-1.asm')
      .toString().replace(/\r?\n/mg, " ").split(" ").filter(word => word);
  
  console.log(words);

  let isntructions: Instruction[] = [];
  for(let i = 0;i < words.length;i++) {
    const instruction: Instruction = {
      instruction: words[i] as Instructions,
      indirect: false,
    };
    if(instruction.instruction.endsWith(':')) {
      i += 2;
      instruction.argument = tryInt(words[i]);
    } else if(!['HLT'].includes(instruction.instruction)) {
      i++;
      instruction.argument = tryInt(words[i]);
    }
    isntructions.push(instruction);
  }
  console.log(words);
  console.log(isntructions);

  let locations: {[key: string]: number} = {};
  let names: string[] = [];
  isntructions.map((inst, i) => {
    if(!inst.instruction.endsWith(':')) return inst;
    const name = inst.instruction.substring(0, inst.instruction.length - 1);
    delete inst.instruction;
    names.push(name);
    locations[name] = i;
    return inst;
  })

  names.sort((a, b) => b.length - a.length);
  console.log(isntructions);

  isntructions.map(inst => {
    if(typeof inst.argument !== 'string') return inst;
    if(inst.argument.includes("[") && inst.argument.includes("]")) {
      inst.indirect = true;
      inst.argument = inst.argument.replace('[', '').replace(']', '');
    }
    for(const name of names) {
      if(inst.argument.includes(name)) {
        inst.argument = tryInt(inst.argument.replace(name, locations[name].toString()));
        return inst;
      }
    }
    return inst;
  })
  console.log(isntructions);

  let buffer = new Uint16Array(isntructions.length > 4096 ? 4096 : isntructions.length);
  isntructions.forEach((inst, i) => {
    buffer[i] = 0;
    if(!inst.instruction) {
      buffer[i] = parseInt(inst.argument as string);
    } else {
      if(inst.indirect) {
        buffer[i] |= InstructionTypes.I;
      }
      const type = Object.keys(InstructionTypes).filter(key => key === inst.instruction)[0];
      buffer[i] |= InstructionTypes[type];
      if(inst.argument) buffer[i] |= inst.argument as number;
    }
    let result;
    result = buffer[i] >> 8;
    result += (buffer[i] & 0xFF) << 8;
    buffer[i] = result;
  });

  function byteToBinaryString(s) {
    return s.toString(2).padStart(16, '0');
  }

  console.log(
    [...buffer].filter((_, i) => i < 10).map(byteToBinaryString).join(" ")
  );

  // writeFileSync('program2.bin', buffer);
  writeFileSync('program.bin', buffer);
  console.log(buffer);

}

run();

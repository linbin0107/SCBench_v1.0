#include <iostream>
#include "Y86.h"

void cpu_Y86::stage_0_pre_assign()
{
  sc_uint<5> ue_stage_0;//LCOV_EXCL_BR_LINE
  wait();//LCOV_EXCL_BR_LINE
  while(true)
  {
    //LCOV_EXCL_BR_START
    ue_stage_0.range(4, 1)=full.range(3, 0);
    ue_stage_0[0]=full[4];

    ue.write(ue_stage_0);

    wait();
    //LCOV_EXCL_BR_STOP
  }
}

  // stage 0
void cpu_Y86::stage_0()
{
  for(unsigned i=1; i<=4; i++)
    full[i]=false;
  full[0] = 1;

  wait(2);//LCOV_EXCL_BR_LINE
  while(true)
  {
    for(unsigned i=5; i>0; i--)
      full[i-1]=ue.read()[i-1];//LCOV_EXCL_BR_LINE

    wait();//LCOV_EXCL_BR_LINE
  }
}

  // stage 1 IF method
void cpu_Y86::process_IF()
{
  bool en = ue.read()[0];//LCOV_EXCL_BR_LINE
  if(en){
    IR=bus_in_.read();//LCOV_EXCL_BR_LINE
    full[1] = 1;
  }
}

  // stage 2 ID method
void cpu_Y86::process_ID_pre_assign()
{
  sc_uint<32> v_stage_2;//LCOV_EXCL_BR_LINE
  wait();//LCOV_EXCL_BR_LINE
  while(true)
  {
    //LCOV_EXCL_BR_START
    opcode.write(IR.range(7,0));
    cerr << "opcode: " << IR.range(7,0)<<endl;
    mod.write(IR.range(15,14));
    cerr << "mod: " << IR.range(15,14)<<endl;
    load.write((opcode.read()==0x8b) && (mod.read()==1));
    move.write((opcode.read()==0x89) && (mod.read()==3));
    store.write((opcode.read()==0x89) && (mod.read()==1));
    memory.write(load.read()||store.read());
    add.write(opcode.read()==0x01);
    sub.write(opcode.read()==0x29);
    halt.write(opcode.read()==0xF4);
    aluop.write((add.read())||(sub.read()));
    jnez.write(opcode.read()==0x75);

    RD.write(IR.range(10,8));
    RS.write(IR.range(13,11));
    //LCOV_EXCL_BR_STOP
    if(memory.read()==true)
      Aad.write(6);//LCOV_EXCL_BR_LINE
    else
      Aad.write(RD.read());//LCOV_EXCL_BR_LINE

    Bad.write(RS.read());//LCOV_EXCL_BR_LINE

    for(unsigned i=31; i>=8; i--)
      v_stage_2[i]=IR[15];//LCOV_EXCL_BR_LINE
    //for(unsigned i=7; i>=0; i--)
      //v_stage_2[i]=IR[i+8];
    for(unsigned i=8; i>0; i--)
      v_stage_2[i-1]=IR[i+8-1];//LCOV_EXCL_BR_LINE

    distance.write(v_stage_2);//LCOV_EXCL_BR_LINE

    for(unsigned i=31; i>=8; i--)
      v_stage_2[i]=IR[23];//LCOV_EXCL_BR_LINE
    //for(unsigned i=7; i>=0; i--)
    //  v_stage_2[i]=IR[i+16];
    for(unsigned i=8; i>0; i--)
      v_stage_2[i-1]=IR[i+16-1];//LCOV_EXCL_BR_LINE

    displacement.write(v_stage_2);//LCOV_EXCL_BR_LINE

    btaken.write(jnez.read()&&(ZF==false));//LCOV_EXCL_BR_LINE

    if(memory.read())
      length.write(3);//LCOV_EXCL_BR_LINE
    else
    {
      if(aluop.read()||move.read()||jnez.read())
        length.write(2);//LCOV_EXCL_BR_LINE
      else
        length.write(1);//LCOV_EXCL_BR_LINE
    }

    wait();//LCOV_EXCL_BR_LINE
  }
}

void cpu_Y86::process_ID()
{
  bool reset = rst_.read();//LCOV_EXCL_BR_LINE

  if(reset)
    IP=0x0000;
  else if(ue.read()[1])
  {
    A=Aop.read();//LCOV_EXCL_BR_LINE
    B=Bop.read();//LCOV_EXCL_BR_LINE

    bool isHalt = halt.read();//LCOV_EXCL_BR_LINE
    if(isHalt==false)
      IP=IP+length.read();//LCOV_EXCL_BR_LINE

    if(btaken.read()==true)
      IP=IP+distance.read();//LCOV_EXCL_BR_LINE
  }
}

  // stage 3 EX method
void cpu_Y86::process_EX_pre_assign()
{
  wait();//LCOV_EXCL_BR_LINE
  while(true)
  {
    if(memory.read())
      ALU_op2.write(displacement.read());//LCOV_EXCL_BR_LINE
    else
      if(sub.read())
        ALU_op2.write(B^0xFFFF);//LCOV_EXCL_BR_LINE
      else
        ALU_op2.write(B);//LCOV_EXCL_BR_LINE

    ALUout.write(A+ALU_op2.read()+sub.read());//LCOV_EXCL_BR_LINE

    wait();//LCOV_EXCL_BR_LINE
  }
}

void cpu_Y86::process_EX()
{
  bool reset = rst_.read();//LCOV_EXCL_BR_LINE
  if(reset)
    ZF=false;
  else if(ue.read()[2])
  {
    MAR=ALUout.read();//LCOV_EXCL_BR_LINE
    C=(move.read())?B:ALUout.read();
    MDRw=B;
    if(aluop.read())
      ZF=(ALUout.read()==0x0000);//LCOV_EXCL_BR_LINE
  }
}

 // stage 4 MEM method
void cpu_Y86::process_MEM()
{
  if(ue.read()[3]&&load.read())
    MDRr=bus_in_.read();//LCOV_EXCL_BR_LINE

  if(ue.read()[3])
    bus_A_.write(MAR);//LCOV_EXCL_BR_LINE
  else
  {
    if(ue.read()[0])
      bus_A_.write(IP);//LCOV_EXCL_BR_LINE
    else
      bus_A_.write(0x0000);//LCOV_EXCL_BR_LINE
  }

  bus_RE_.write(ue.read()[0]||(ue.read()[3]&&load.read()));//LCOV_EXCL_BR_LINE
}

 // stage 4 MEM method
void cpu_Y86::process_WB_pre_assign()
{
  wait();//LCOV_EXCL_BR_LINE
  while(true)
  {
    int a = Aad.read();
    int b = Bad.read();
    cout<<"a: " << a<<endl;
    cout<<"b: " << b<<endl;
    cout<<"R[0]: "<<R[a] << endl;
    //LCOV_EXCL_BR_START
    Aop.write(R[Aad.read()]);
    Bop.write(R[Bad.read()]);

    bus_WE_.write(ue.read()[3] && store.read());
    bus_out_.write(MDRw);

    wait();
    //LCOV_EXCL_BR_STOP
  }
}

void cpu_Y86::process_WB()
{
  bool reset = rst_.read();//LCOV_EXCL_BR_LINE
  if(reset == true)
  {
    R[0x00]=0x00;
    R[0x01]=0x00;
    R[0x02]=0x00;
    R[0x03]=0x00;
    R[0x04]=0x00;
    R[0x05]=0x00;
    R[0x06]=0x00;
    R[0x07]=0x00;
  }
  else
  {
    if(ue.read()[4]==true) {
      bool cond = aluop.read()||move.read()||load.read();//LCOV_EXCL_BR_LINE
      if(cond)
      {
        bool load_tmp = load.read();//LCOV_EXCL_BR_LINE
        if(load_tmp==true)
	      R[RS.read()]=MDRr;//LCOV_EXCL_BR_LINE
	    else
	      R[RD.read()]=C;//LCOV_EXCL_BR_LINE
      }
    }
  }
}

void cpu_Y86::process_WB_aft_assign()
{
  wait();//LCOV_EXCL_BR_LINE
  while(true)
  {
    //LCOV_EXCL_BR_START
    current_opcode_.write(opcode.read());
    eax.write(R[0]);
    ecx.write(R[1]);
    edx.write(R[2]);
    ebx.write(R[3]);
    esp.write(R[4]);
    ebp.write(R[5]);
    esi.write(R[6]);
    edi.write(R[7]);
    regs.write(IR.range(15, 8));

    wait();
    //LCOV_EXCL_BR_STOP
  }
}//LCOV_EXCL_BR_LINE

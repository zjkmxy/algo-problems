import java.io.*;
import java.text.DecimalFormat;
import java.util.*;
import java.lang.Math;

class Complex{
	public final double re, im;
	public static final double eps = 1e-9;
	public static boolean isZero(double x){
		return (x < eps) && (x > - eps);
	}
	public Complex(double re, double im){
		this.re = re;
		this.im = im;
	}
	public double getMod(){
		return (re * re + im * im);
	}
	public double getArg(){
		return Math.atan2(im, re);
	}
	public static Complex byVec(double mod, double arg){
		return new Complex(mod * Math.cos(arg), mod * Math.sin(arg));
	}
	public static Complex byVecm(double maxval, double arg){
		return byVec(maxval, arg);
	}
	public Complex add(Complex b){
		return new Complex(re + b.re, im + b.im);
	}
	public Complex sub(Complex b){
		return new Complex(re - b.re, im - b.im);
	}
	public Complex mul(Complex b){
		return new Complex(re * b.re - im * b.im, re * b.im + im * b.re);
	}
	public Complex div(Complex b){
		double mod = b.re * b.re + b.im * b.im;
		return new Complex((re * b.re + im * b.im) / mod, (im * b.re - re * b.im) / mod);
	}
	public Complex conj(){
		return new Complex(re, - im);
	}
	public Complex minus(){
		return new Complex(-re, - im);
	}
	public boolean equal(Complex b){
		return isZero(re - b.re) && isZero(im - b.im);
	}
}

class Data{
	public final Complex vol, curr;
	public Data(Complex vol, Complex curr){
		this.vol = vol;
		this.curr = curr;
	}

	public Complex getSPower(){
		return vol.mul(curr.conj());
	}
}
class Circuit{
	public final double afreq;
	public Block left, right;
	
	public Circuit(double afreq){
		this.afreq = afreq;
		left = right = null;
	}
	
	public abstract class Block{
		protected Combin par;
		public Data eval(){
			if(par != null)
				return par.getVal(this);
			else
				return Circuit.this.eval(this);
		}
		public abstract Complex getVol();
		public abstract Complex getZ();
	}
	public abstract class Combin extends Block{
		protected Block e1, e2;
		public Combin(Block e1, Block e2){
			this.e1 = e1;
			this.e2 = e2;
			e1.par = this;
			e2.par = this;
		}
		public abstract Data getVal(Block e);
	}
	public class Resist extends Block{
		double r;
		public Resist(double r){
			this.r = r;
		}
		public Complex getVol(){
			return new Complex(0.0, 0.0);
		}
		public Complex getZ(){
			return new Complex(r, 0.0);
		}
	}
	public class Capacit extends Block{
		double c;
		public Capacit(double c){
			this.c = c;
		}
		public Complex getVol(){
			return new Complex(0.0, 0.0);
		}
		public Complex getZ(){
			return new Complex(0.0, - 1.0 / c / afreq);
		}
	}
	public class Inductor extends Block{
		double l;
		public Inductor(double l){
			this.l = l;
		}
		public Complex getVol(){
			return new Complex(0.0, 0.0);
		}
		public Complex getZ(){
			return new Complex(0.0, l * afreq);
		}
	}
	public class Supply extends Block{
		Complex vol;
		public Supply(Complex vol){
			this.vol = vol;
		}
		public Supply(double u, double a) {
			this.vol = Complex.byVec(u, a);
		}
		public Complex getVol(){
			return vol;
		}
		public Complex getZ(){
			return new Complex(0.0, 0.0);
		}
	}
	public class Series extends Combin{
		public Series(Block e1, Block e2) {
			super(e1, e2);
		}
		public Complex getVol() {
			return e1.getVol().add(e2.getVol());
		}
		public Complex getZ() {
			return e1.getZ().add(e2.getZ());
		}
		public Data getVal(Block e) {
			Data ret = eval();
			return new Data(e.getVol().sub(ret.curr.mul(e.getZ())), ret.curr);
		}
	}
	public class Parallel extends Combin{
		public Parallel(Block e1, Block e2) {
			super(e1, e2);
		}
		public Complex getVol() {
			if(Complex.isZero(e1.getZ().getMod())){
				return e1.getVol();
			}else if(Complex.isZero(e2.getZ().getMod())){
				return e2.getVol();
			}else{
				Complex i = e1.getVol().div(e1.getZ()).add(e2.getVol().div(e2.getZ()));
				return i.mul(this.getZ());
			}
		}
		public Complex getZ() {
			if(Complex.isZero(e1.getZ().getMod()) || Complex.isZero(e2.getZ().getMod())){
				return new Complex(0.0, 0.0);
			}else{
				Complex i = new Complex(1.0, 0.0);
				return i.div(i.div(e1.getZ()).add(i.div(e2.getZ())));
			}
		}
		public Data getVal(Block e) {
			Data ret = eval();
			if(! Complex.isZero(e.getZ().getMod())){
				return new Data(ret.vol, ret.vol.sub(e.getVol()).div(e.getZ()).minus());
			}else{
				Block other;
				if(e == e1)
					other = e2;
				else
					other = e1;
				return new Data(ret.vol, ret.curr.add(e.getVol().sub(other.getVol()).div(other.getZ())));
			}
		}
	}
	
	private Data eval(Block e){
		if(left == null || right == null){
			return new Data(new Complex(0.0, 0.0), new Complex(0.0, 0.0));
		}else{
			Complex i = left.getVol().sub(right.getVol()).div(left.getZ().add(right.getZ()));
			Complex v = left.getVol().sub(i.mul(left.getZ()));
			if(e == left){
				return new Data(v, i);
			}else{
				return new Data(v, i.minus());
			}
		}
	}
}
public class Main{
	static Circuit cir;
	static Scanner scanner;
	static PrintWriter writer;
	
	static Circuit.Block readBlock(String str){
		char ch = str.charAt(0);
		if(ch == 'R'){
			int pos = str.indexOf(")");
			double r = Double.parseDouble(str.substring(2, pos));
			return cir.new Resist(r);
		}else if(ch == 'C'){
			int pos = str.indexOf(")");
			double c = Double.parseDouble(str.substring(2, pos));
			return cir.new Capacit(c);
		}else if(ch == 'L'){
			int pos = str.indexOf(")");
			double l = Double.parseDouble(str.substring(2, pos));
			return cir.new Inductor(l);
		}else if(ch == 'U'){
			int pos1 = str.indexOf(",");
			int pos2 = str.indexOf(")");
			double u = Double.parseDouble(str.substring(2, pos1));
			double a = Double.parseDouble(str.substring(pos1 + 1, pos2));
			return cir.new Supply(u, a);
		}else{
			int pos2 = str.lastIndexOf(")");
			int pos1, flr = 0;
			for(pos1 = 2; pos1 < pos2; pos1 ++){
				if(str.charAt(pos1) == '(')
					flr++;
				else if(str.charAt(pos1) == ')')
					flr--;
				else if(flr == 0 && str.charAt(pos1) == ',')
					break;
			}
			Circuit.Block e1 = readBlock(str.substring(2, pos1));
			Circuit.Block e2 = readBlock(str.substring(pos1 + 1, pos2));
			if(ch == '+')
				return cir.new Series(e1, e2);
			else
				return cir.new Parallel(e1, e2);
		}
	}
	
	static public void main(String [] args){
		double f;
		Complex s;
		DecimalFormat df;
		scanner=new Scanner(System.in);
		writer = new PrintWriter(System.out);
		f = scanner.nextDouble();
		cir = new Circuit(f);
		cir.left = readBlock(scanner.next());
		cir.right = readBlock(scanner.next());
		s = cir.left.eval().getSPower();
		df = new DecimalFormat("#0.00");
		writer.print(df.format(s.re));
		writer.print("W ");
		writer.print(df.format(s.im));
		writer.print("Var\n");
		writer.flush();
		writer.close();
	}
}
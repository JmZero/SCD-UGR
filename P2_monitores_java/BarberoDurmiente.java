import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor
{
	private int numero_clientes;
	private boolean barberoOcupado;
	private boolean sillaOcupada;

	private Condition cliente_sala_espera = makeCondition();
	private Condition barbero = makeCondition();
	private Condition cliente_ocupado = makeCondition();

	public Barberia(){
		barberoOcupado = false;
		sillaOcupada = false;
		numero_clientes = 0;
	}

	public void cortarPelo(){

		enter();
		
		numero_clientes++;

		if(barberoOcupado || numero_clientes > 1){
			System.out.println("Un cliente pasa a la sala de espera porque el barbero está ocupado. Hay " + Integer.toString(numero_clientes-1) + " clientes esperando.");
			cliente_sala_espera.await();
		}

		else{
			System.out.println("Un cliente despierta al barbero");
			barbero.signal();
		}
		
		cliente_ocupado.await();
		numero_clientes--;

		leave();
	}

	public void siguienteCliente(){
	
		enter();

		if(numero_clientes == 0){
			System.out.println("No hay ningún cliente esperando y el barbero se va a dormir");
			barbero.await();
		}

		else{	
			System.out.println("El barbero llama a un cliente"); 
			cliente_sala_espera.signal();
		}
		
		barberoOcupado = true;
		System.out.println("El barbero comienza a pelar al cliente");

		leave();
	}

	public void finCliente(){
		
		enter();
		
		System.out.println("El barbero termina de pelar al cliente y el cliente sale por la puerta");
		cliente_ocupado.signal();
			
		leave();
	}
}

class Barbero implements Runnable
{
	private Barberia barberia;
	public Thread thr;
	
	public Barbero(Barberia bbarberia){
		barberia = bbarberia;
		thr = new Thread(this,"barbero");
	}

	public void run(){

		while(true){

			barberia.siguienteCliente();
			aux.dormir_max(2500);
			barberia.finCliente();

		}
	}
}

class Clientes implements Runnable
{
	private Barberia barberia;
	public Thread thr;

	public Clientes(Barberia bbarberia){
		barberia = bbarberia;
		thr = new Thread(this,"cliente");
	}

	public void run(){

		while(true){

			barberia.cortarPelo();
			aux.dormir_max(2000);

		}

	}
}

class aux
{
  static Random genAlea = new Random() ;
  static void dormir_max( int milisecsMax )
  { 
    try
    { 
      Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    }
    catch( InterruptedException e )
    { 
      System.err.println("sleep interumpido en ’aux.dormir_max()’");
    }
  }
}

class BarberoDurmiente
{
	public static void main(String[] args)
	{
		
		final int NUM_CLIENTES = 5;

		Barberia barberia = new Barberia();
		Thread barbero = new Thread (new Barbero(barberia));
		
		Thread[] clientes = new Thread[NUM_CLIENTES];
		for(int i = 0; i < NUM_CLIENTES; ++i)
			clientes[i] = new Thread (new Clientes(barberia));

		barbero.start();

		for(int i = 0; i < NUM_CLIENTES; ++i)
			clientes[i].start();

	}
}

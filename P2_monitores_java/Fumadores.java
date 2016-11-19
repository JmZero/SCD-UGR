import monitor.*;
import java.util.Random;

class Estanco extends AbstractMonitor
{
	private int ingrediente_pasado;
	private boolean ingrediente_recogido;
	private Condition[] fumador = new Condition[3];
	private Condition estanquero;

	public Estanco(){
		estanquero = makeCondition();
		for(int i = 0; i < 3; ++i)
			fumador[i] = makeCondition();
		ingrediente_recogido = false;
		ingrediente_pasado = -1;
	}

	// invocado por cada fumador, indicando su ingrediente o numero
	
	public void obtenerIngrediente( int miIngrediente )
	{
		enter();

		if(ingrediente_pasado != miIngrediente)
			fumador[miIngrediente].await();
		
		ingrediente_recogido = true;
		
		if(miIngrediente == 0)
			System.out.println("El fumador 0 comienza a fumar");		

		else if (miIngrediente == 1)
			System.out.println("El fumador 1 comienza a fumar");

		else if (miIngrediente == 2)
			System.out.println("El fumador 2 comienza a fumar");
		
		estanquero.signal();

		leave();
	}

	// invocado por el estanquero, indicando el ingrediente que pone

	public void ponerIngrediente( int ingrediente )
	{
		enter();
		
		if(ingrediente == 0)
			System.out.println("El estanquero pone papel");
		
		else if(ingrediente == 1)
			System.out.println("El estanquero pone tabaco");

		else
			System.out.println("El estanquero pone cerillas");
		
		ingrediente_recogido = false;
		ingrediente_pasado = ingrediente;
		fumador[ingrediente].signal();
		
		leave();
	}	

	// invocado por el estanquero
	public void esperarRecogidaIngrediente()
	{
		enter();
			
		if(!ingrediente_recogido)
			estanquero.await();
		
		leave();
	}
}

class Fumador implements Runnable
{
	private int miIngrediente;
	private Estanco estanco;
	public Thread thr ;

	public Fumador( int p_miIngrediente, Estanco p_estanco )
	{ 
		miIngrediente = p_miIngrediente;
		estanco = p_estanco;
		thr = new Thread(this,"fumador "+miIngrediente);
	}

	public void run()
	{
		while ( true )
		{ 
			estanco.obtenerIngrediente( miIngrediente );
			aux.dormir_max(1500);
			System.out.println("El fumador " + miIngrediente + " termina de fumar");

		}
	}
}

class Estanquero implements Runnable
{ 
	public Thread thr ;
	private Estanco estanco;

	public Estanquero(Estanco p_Estanco){
		estanco = p_Estanco;
		thr = new Thread(this,"estanquero");
	}

	public void run()
	{ 
		int ingrediente ;
		while (true)
		{
			ingrediente = (int) (Math.random () * 3.0); // 0,1 o 2
			estanco.ponerIngrediente(ingrediente);
			estanco.esperarRecogidaIngrediente() ;
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

class Fumadores
{
  public static void main( String[] args )
  {
		
	 Estanco estanco = new Estanco();
	 Thread hebraEstanquero = new Thread(new Estanquero(estanco));
	 Thread[] hebraFumador = new Thread[3];

    // crear hebras de fumadores
    for(int i = 0; i < 3; i++)
      hebraFumador[i] = new Thread(new Fumador(i,estanco));

    // poner en marcha las hebras
    hebraEstanquero.start();
	 for(int i = 0; i < 3; i++)
      hebraFumador[i].start();
  }
}

#include <fl/Headers.h>

using namespace fl;

int main()
{
	Engine* engine = new Engine();

	InputVariable* obstacle = new InputVariable;
	InputVariable* speed = new InputVariable;

	//obstacle
	engine->addInputVariable(obstacle);

	obstacle->setName("obstacle");
	obstacle->setRange(0.000, 1.000);

	obstacle->addTerm(new Ramp("left", 1.000, 0.000));
	obstacle->addTerm(new Ramp("right", 0.000, 1.000));
	
	//speed
	engine->addInputVariable(speed);

	speed->setName("speed");
	speed->setRange(0.000, 1.000);

	speed->addTerm(new Ramp("moving_left", 1.000, 0.700));
	speed->addTerm(new Ramp("moving_right", 0.300, 1.000));
	speed->addTerm(new Triangle("none", 0.200, 0.500, 0.800));

	//output Variable
	OutputVariable* mSteer = new OutputVariable;
	engine->addOutputVariable(mSteer);
	mSteer->setName("mSteer");
	mSteer->setRange(0.000, 1.000);
	mSteer->setAggregation(new Maximum);
	mSteer->setDefuzzifier(new Centroid(100));
	mSteer->setDefaultValue(fl::nan);
	mSteer->addTerm(new Ramp("left", 1.000, 0.000));
	mSteer->addTerm(new Ramp("right", 0.000, 1.000));

	//Rule Block
	RuleBlock* mamdani = new RuleBlock;
	mamdani->setName("mamdani");
	mamdani->setConjunction(new AlgebraicProduct);
	mamdani->setDisjunction(new Maximum);
	mamdani->setImplication(new AlgebraicProduct);
	mamdani->setActivation(new General);
	mamdani->addRule(Rule::parse("if obstacle is left or speed is moving_left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right or speed is moving_right then mSteer is left", engine));
	engine->addRuleBlock(mamdani);
	
	//inputs
	std::cout << "please enter a number (0-1) or type 'exit' to quit " << std::endl;
	std::string input = "";
	std::string input2 = "";
	do {
		std::cin >> input;
		std::cin >> input2;

		std::stringstream ss(input);
		std::stringstream ss2(input2);
		float number = 0.f;
		float number2 = 0.f;
		ss >> number;
		ss2 >> number2;
		obstacle->setValue(number);
		speed->setValue(number2);
		engine->process();
		std::cout << "obstacle.input = " << number << "speed.input =" << number2 << " => steer.output = " << mSteer->getValue() << std::endl;
	} while(input != "exit");


	delete engine;
	return 0;
}
//Header
#include "model.hpp"

vector<string> models = {"none", "simple", "regular", "complex"};
vector<string> randoms = {"uniform", "gaussian"};

mdl::mdl(cfg config)
{
	#if(MODEL)
	level = find(models.begin(), models.end(), config.getValueString("model")) - models.begin();
	if(level == models.size())
		throw "Error: Unknow Model Level";
	#else
	level = 1;
	#endif
	type = find(randoms.begin(), randoms.end(), config.getValueString("noise")) - randoms.begin();
	if(type == randoms.size())
		throw "Error: Unknow Noise Type";
	wsmiu = dvec3(config.getValueVectorDouble("meanwind", 0), config.getValueVectorDouble("meanwind", 1), config.getValueVectorDouble("meanwind", 2));
	wssigma = config.getValueDouble("deviationwind");
	g = config.getValueDouble("gravity");
	m = config.getValueDouble("mass");
	k = config.getValueDouble("airfrictioncoefficient");
	l = config.getValueDouble("lengtharm");
	I = dmat3(config.getValueVectorDouble("momentinertia", 0), 0.0, 0.0, 0.0, config.getValueVectorDouble("momentinertia", 1), 0.0, 0.0, 0.0,  config.getValueVectorDouble("momentinertia", 2));
	kl = config.getValueDouble("liftcoefficient");
	kd = config.getValueDouble("dragcoefficient");
	ir = config.getValueDouble("momentinertiarotor");
	rm = config.getValueDouble("relationmotorgear");
	km = config.getValueDouble("gainmotorcoefficient");
	taom = config.getValueDouble("timemotorcoefficient");
	M = dmat4(kl, -sqrt(2.0) / 2.0 * l * kl, -sqrt(2.0) / 2.0 * l * kl, -kd, kl, -sqrt(2.0) / 2.0 * l * kl, sqrt(2.0) / 2.0 * l * kl, kd, kl, sqrt(2.0) / 2.0 * l * kl, sqrt(2.0) / 2.0 * l * kl, -kd, kl, sqrt(2.0) / 2.0 * l * kl, -sqrt(2.0) / 2.0 * l * kl, kd);
	xsigma = config.getValueDouble("deviationposition");
	thetasigma = config.getValueDouble("deviationattitude") * (M_PI / 180.0);
	xmax = dvec3(config.getValueVectorDouble("maxposition", 0), config.getValueVectorDouble("maxposition", 1), config.getValueVectorDouble("maxposition", 2));
	thetamax = dvec3(config.getValueVectorDouble("maxattitude", 0), config.getValueVectorDouble("maxattitude", 1), config.getValueVectorDouble("maxattitude", 2)) * (M_PI / 180.0);
	vmax = dvec3(config.getValueVectorDouble("maxlinearvelocity", 0), config.getValueVectorDouble("maxlinearvelocity", 1), config.getValueVectorDouble("maxlinearvelocity", 2));
	omegamax = dvec3(config.getValueVectorDouble("maxangularvelocity", 0), config.getValueVectorDouble("maxangularvelocity", 1), config.getValueVectorDouble("maxangularvelocity", 2)) * (M_PI / 180.0);
	wmin = config.getValueDouble("minspeedmotor") * rm * 2.0 * M_PI / 60.0;
	wmax = config.getValueDouble("maxspeedmotor") * rm * 2.0 * M_PI / 60.0;
	w2min = pow(wmin, 2.0);
	w2max = pow(wmax, 2.0);
}

void mdl::update(void)
{
	if(wssigma)
		ws = gaussRand(wsmiu, dvec3(wssigma));
	else
		ws = wsmiu;
}

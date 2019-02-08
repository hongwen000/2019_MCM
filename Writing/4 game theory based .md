## 4 The game theory based individual behaviour simulation model

We construct game theory based individual behaviour simulation model to estimate the optimum strategy for evacuation in micro scope.

### 4.1Theoretical Framework

​       In a complete information dynamic game, an individual will constantly change their options based on other’s decision, knowing the utility payoff and thus the choice of other players. 

Within a game-theory-based scenario, a rational individual will choose the strategy that could maximize his/her utility payoff – in this case, everyone wants to escape this building as soon as possible, regardless of the overall evacuation time. In the simpliest case, an individual will choose the closest exit in order to minimize escape time, but when the number of escapers is great enough, one must take into account the number of potential ‘rivals’ that will possibly choose the same exit, because other escapers could cause conjestion and thus extra escape time. Since no one knows exactly how many people will choose the same exit as he/she chooses, the dynamic situation where all participants strive to find the best route for themselves on the basis of other’s choices is a game and the situation where everyone has no incentive to change his/her decision is a *Nash Equilibrium*.  

​       In this model, we try to include both the consideration of distance to exit and the consideration of congestion as criteria to calculate utility payoff of each participant. After a dynamic adjustment process, an equilibrium is achieved. 

### 4.2 Model Analysis

#### 4.2.1 Model Description

​       In this model, we assign some potential gathering points such as sightseeing sites that initially hold a larger number of people. In order to assess the utility payoff of each participant in different initial points choosing specific egress, we introduce the *cost* of choosing this route. We assume each corridor has a *width* and a $length$ , where $width$  stands for the transactional capacity of this corridor and $length$ stands for the horizonal capacity. We define the density of this corridor as $rho$ , which is defined as $\rho= \frac{num}{width*length}$ where $num$ is the number of people choosing this route. 

It is reasonable to assume that the more people choose this route the slower this route might be, which means the passage velocity is closely related to the number of people who choose this route. By applying the ____ formular, the velocity of this corridor *v* can be calculated in three subsets: one is constant at 1.4 when $rho$  is smaller than 0.75, meaning everyone’s passage is not influenced by others; one is calculated by $1.867 - 0.59*rho - 0.0412rho^2* $ when   $rho$ is greater than 0.75  and smaller than 4.2; the last one is close to 0 when  $rho$ is greater than 4.2, meaning the corridor is close to complete blockage due to corridor overload.

Finally, the $cost$  can be calculated by dividing *length* by $v$ , denoted as $\frac{lenth}{v}$

 The variable $cost$  refer to the cost of time trying to escape by using the specific route the participant chooses. The value of the utility payoff of the participants choosing a specific route is negatively related to the cost of choosing this route. We can thus maximize the utility payoff of a participant by minimizing the *cost* of the specific strategy.

#### 4.2.2 Model Iteration

​       We obtain the average visit per day by calculating the mathematic mean of the previous 4 years’ visits to the Louvre, and subsequently assign the same numbers of people to each gathering point. The second step is to set a random route preference for each participant as a begin point. Because the change of one person’s choice will also change the velocity of a corridor and thus change the utility payoff of others, this process of iteration is a constantly adjusting process. We continue iterating until no one has a better option – no one has a lower cost of getting out by choosing a different route. At this time, we reach a equilibrium where under a complete rationality assumption everyone ceases to alter their choices.

The model above serves as our preliminary model due to its limitation and next step we will introduce another model to solve the problem comprehensively.
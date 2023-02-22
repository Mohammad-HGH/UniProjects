import DUMMY_MEALS from '../../../DataBase/database.json'
import classes from './AvailableMeals.module.css'
import Card from '../UI/Card'
import MealItem from './MealsItem/MealItem'


const AvailableMeals = () => {

    const mealsList = DUMMY_MEALS.map( ( meal ) => (
        <MealItem
            key={ meal.id }
            id={ meal.id }
            name={ meal.name }
            description={ meal.description }
            price={ meal.price }
        />
    ) )
    return (
        <section className={ classes.meals }>
            <Card>
                <ul>
                    { mealsList }
                </ul>
            </Card>
        </section>
    )
}
export default AvailableMeals;
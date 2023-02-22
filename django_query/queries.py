from xml.dom.minidom import Attr
from .models import ModelName


# all values in table return
queryset = ModelName.objects.all()

# select a column
queryItem = ModelName.objects.get(attribute='value')

querySet = ModelName.object.filter(attribute='value') # column  value
                           .filter(attribute__startswith='value')
                           .filter(attribute__contains='value') # casesensitive
                           .filter(attribute__icontains='value') # without casesensitive
                           .filter(attribute__gt='value') # greater than
                           .filter(attribute__gte='value') # greater than equal
                           .filter(attribute__lt='value') # less than
                           .filter(atribute__lte='value') # less than equal


querySet = ModelName.object.exclude(attribute='value') # all expect column value
querySet = Project.object.filter(title="first project").order_by('value1','value2') # which title is = first project and order output by value1 and value2
querySet = Project.object.filter(title="first project").order_by('-value1','-value2') # which title is = first project and order output reverse by value1 and value2

# create an instance of model
item = ModelName.objects.create(attribute='value')

# save changes
item = ModelName.objects.get(attribute='value')
item.title = 'new value'
item.save()


# deletes particular obj
item = ModelName.objects.last()
item.delete

# in 1-n relation query models children
item = ModelName.object.first()
item.childmodel_set.all()

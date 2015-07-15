from django.conf.urls import include, url
from django.contrib import admin
from rasGPIO.views import *

urlpatterns = [
    # Examples:
    # url(r'^$', 'raspberryGPIO.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    #url(r'^admin/', include(admin.site.urls)),
    url(r'^$', 'rasGPIO.views.gpio', name='gpio')

]